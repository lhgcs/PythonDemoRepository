#include "playmp3.h"

#include <stdio.h>
#include <stdlib.h>
#include "alsa/asoundlib.h"

struct WAV_HEADER
{
    char rld[4];            //riff 标志符号
    int rLen;
    char wld[4];            //格式类型（wave）
    char fld[4];            //"fmt"

    int fLen;               //sizeof(wave format matex)
    short wFormatTag;       //编码格式
    short wChannels;        //声道数
    int   nSamplesPersec ;  //采样频率
    int   nAvgBitsPerSample;//WAVE文件采样大小
    short wBlockAlign;      //块对齐
    short wBitsPerSample;   //WAVE文件采样大小

    char  dld[4];           //”data“
    int   wSampleLength;    //音频数据的大小

} wav_header;

/*
nread=44
文件大小rLen：0
声道数：30305
采样频率：775042406
采样的位数：-7941
wSampleLength=6881280
frames:2048
datablock:-256
size:8192
*/

PlayMp3::PlayMp3(){
    playFlag=false;
}

void PlayMp3::play(){
    if(fileName == NULL){
            printf("Usage:wav-player+wav file name\n");
            exit(1);
        }

        int nread;
        FILE *fp;
        fp=fopen(fileName,"rb");
        if(fp==NULL){
            perror("open file failed:\n");
            exit(1);
        }

        nread=fread(&wav_header,1,sizeof(wav_header),fp);
        printf("nread=%d\n",nread);

        //printf("RIFF 标志%s\n",wav_header.rld);
        printf("文件大小rLen：%d\n",wav_header.rLen);
        //printf("wld=%s\n",wav_header.wld);
        //printf("fld=%s\n",wav_header.fld);

       // printf("fLen=%d\n",wav_header.fLen);

        //printf("wFormatTag=%d\n",wav_header.wFormatTag);
        printf("声道数：%d\n",wav_header.wChannels);
        printf("采样频率：%d\n",wav_header.nSamplesPersec);
        //printf("nAvgBitsPerSample=%d\n",wav_header.nAvgBitsPerSample);
        //printf("wBlockAlign=%d\n",wav_header.wBlockAlign);
        printf("采样的位数：%d\n",wav_header.wBitsPerSample);

       // printf("data=%s\n",wav_header.dld);
        printf("wSampleLength=%d\n",wav_header.wSampleLength);

        /*
        文件大小rLen：34397808
        声道数：2
        采样频率：44100
        采样的位数：16
        wSampleLength=34397772
        frames:1024
        datablock:4
        size:2048
        */
        set_pcm_play(fp);
}

void PlayMp3::setFileName(char *value)
{
    fileName = value;
}

void PlayMp3::setPlayFlag(volatile bool value)
{
    playFlag = value;
}

volatile bool PlayMp3::getPlayFlag() const
{
    return playFlag;
}


int PlayMp3::set_pcm_play(FILE *fp)
{
    int    rc;
    int    ret;
    int    size;
    snd_pcm_t*           handle;         //PCI设备句柄
    snd_pcm_hw_params_t* params;//硬件信息和PCM流配置
    unsigned int         val;
    int                  dir=0;
    snd_pcm_uframes_t    frames;
    char*                buffer;
    int channels=wav_header.wChannels;
    int frequency=wav_header.nSamplesPersec;
    int bit=wav_header.wBitsPerSample;
    int datablock=wav_header.wBlockAlign;
    //unsigned char ch[100];  //用来存储wav文件的头信息

    if(fp == NULL){
        return 0;
    }
    //1. 打开PCM，最后一个参数为0意味着标准配置
    rc=snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if(rc<0){
            perror("\nopen PCM device failed:");
            exit(1);
    }
    //2. 分配snd_pcm_hw_params_t结构体
    rc=snd_pcm_hw_params_malloc(&params);// snd_pcm_hw_params_alloca(params); //分配params结构体
    if(rc<0){
            perror("\nsnd_pcm_hw_params_alloca:");
            exit(1);
    }
    //3. 初始化hw_params
    rc=snd_pcm_hw_params_any(handle, params);
    if(rc<0){
            perror("\nsnd_pcm_hw_params_any:");
            exit(1);
    }
    //4. 初始化访问权限
    rc=snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);                                 //初始化访问权限
    if(rc<0){
            perror("\nsed_pcm_hw_set_access:");
            exit(1);
    }
    //5. 初始化采样格式SND_PCM_FORMAT_U8,8位
    switch(bit/8){//采样位数
        case 1:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_U8);    break ;
        case 2:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);break ;
        case 3:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S24_LE);break ;
        default:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);break ;
    }
    //7. 设置通道数量
    rc=snd_pcm_hw_params_set_channels(handle, params, channels);  //设置声道,1表示单声>道，2表示立体声
    if(rc<0){
            perror("\nsnd_pcm_hw_params_set_channels:");
            exit(1);
    }
    //6. 设置采样率，如果硬件不支持我们设置的采样率，将使用最接近的
    //val = 44100,有些录音采样频率固定为8KHz
    val = frequency;
    rc=snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);
    if(rc<0){
            perror("\nsnd_pcm_hw_params_set_rate_near:");
            exit(1);
    }

   // Set period size to 32 frames.
   frames = 32;
   long unsigned int periodsize = frames * 2;
   rc = snd_pcm_hw_params_set_buffer_size_near(handle, params, &periodsize);
   if (rc < 0){
        printf("Unable to set buffer size %li : %s\n", frames * 2, snd_strerror(ret));
   }
   periodsize /= 2;

   rc = snd_pcm_hw_params_set_period_size_near(handle, params, &periodsize, 0);
   if (rc < 0){
       printf("Unable to set period size %li : %s\n", periodsize,  snd_strerror(ret));
   }

    //8. 设置hw_params
    rc = snd_pcm_hw_params(handle, params);
    if(rc<0){
        perror("\nsnd_pcm_hw_params: ");
        exit(1);
    }
    /* Set period size to 32 frames. */
    frames = 32;
    rc=snd_pcm_hw_params_get_period_size(params, &frames, &dir);  /*获取周期长度*/
    if(rc<0){
            perror("\nsnd_pcm_hw_params_get_period_size:");
            exit(1);
    }

    //MP3格式获取错误的datablock
    if(datablock <0 ){
        datablock=4;
    }
    size = frames * datablock;   /*4 代表数据快长度*/
    printf("frames:%ld\n",frames);
    printf("datablock:%d\n",datablock);
    printf("size:%d\n",size);

    buffer =(char*)malloc(size);
    if(buffer == NULL){
        return 0;
    }
    fseek(fp,58,SEEK_SET);    //定位歌曲到数据区

    while (playFlag){
        memset(buffer,0,size);//不能用sizeof
        ret = fread(buffer, 1, size, fp);
        if(ret == 0){
            printf("歌曲写入结束\n");
            break;
        }else if (ret != size){
            printf("最后一块数据\n");
            continue;
        }
        //9. 写音频数据到PCM设备
        while((ret = snd_pcm_writei(handle, buffer, frames)) < 0){
            usleep(2000);

            if(ret <0 ){
                switch (ret) {
                    case -EPIPE:
                        /* EPIPE means underrun */
                        fprintf(stderr, "underrun occurred\n");
                        //完成硬件参数设置，使设备准备好
                        snd_pcm_prepare(handle);
                        break;
                    case -EBADFD:    //错误
                    case -ESTRPIPE:  //挂起
                        fprintf(stderr,"error from writei: %s\n",snd_strerror(ret));
                        if(playFlag == true){
                            break;
                        }
                        break;
                    default:
                        printf("未知错误:%d\n",ret);
                        break;
                }
            }
        }
    }
    //10. 关闭PCM设备句柄
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(buffer);
    playFlag=false;
    return 0;
}



//# include <stdio.h>
//# include <unistd.h>
//# include <sys/stat.h>
//# include <sys/mman.h>
//# include <sys/fcntl.h>
//# include "mad.h"
//# include "alsa/asoundlib.h"

//static int decode(unsigned char const *, unsigned long);
//int set_pcm_mp3(snd_pcm_t* handle,snd_pcm_hw_params_t* params);
//void set_volume(long volume);

//snd_pcm_t* handle=NULL;          //PCI设备句柄
//snd_pcm_hw_params_t* params=NULL;//硬件信息和PCM流配置

//int play_mp3(char *fileName)
//{

//    long volume=100;

//    struct stat stat;
//    void *fdm;

//    int fd=open(fileName,O_RDWR);
//    if(fd<0){
//        perror("open file failed:");
//        return 1;
//    }
//    //fstat()用来将fd所指的文件状态，复制到参数buf所指的结构中(struct stat)
//    if (fstat(fd, &stat) == -1 || stat.st_size == 0){
//        printf("fstat failed:\n");
//        return 2;
//    }
//    printf("size:%ld字节--->%.2fM\n",stat.st_size,(float)stat.st_size/1024/1024);

//    fdm = mmap(0, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
//    if (fdm == MAP_FAILED){
//        return 3;
//    }

//    //设置pcm 参数
//    if(set_pcm_mp3(handle,params) != 0) {
//        printf("set_pcm fialed:\n");
//        return 1;
//    }
//    set_volume(volume);
//    decode((unsigned char *)fdm, stat.st_size);

//    if (munmap(fdm, stat.st_size) == -1){
//        return 4;
//    }
//    snd_pcm_drain(handle);
//    snd_pcm_close(handle);
//    return 0;
//}


////alsa设置相关函数
//int set_pcm_mp3(snd_pcm_t* handle,snd_pcm_hw_params_t* params)
//{
//    int rc;
//    int dir=0;
//    unsigned int rate = 44100;          /* 采样频率 44.1KHz*/
//    int format = SND_PCM_FORMAT_S16_LE; /* 量化位数 16 */
//    int channels = 2;                   /* 声道数 2 */

//    rc=snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
//    if(rc<0)    {
//      perror("\nopen PCM device failed:");
//      exit(1);
//    }
//    snd_pcm_hw_params_alloca(&params); //分配params结构体

//    rc=snd_pcm_hw_params_any(handle, params);//初始化params
//    if(rc<0){
//      perror("\nsnd_pcm_hw_params_any:");
//      exit(1);
//    }
//    rc=snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED); //初始化访问权限
//    if(rc<0){
//      perror("\nsed_pcm_hw_set_access:");
//      exit(1);
//    }
//    rc=snd_pcm_hw_params_set_format(handle, params, (enum _snd_pcm_format)format); //设置16位采样精度
//    if(rc<0){
//      perror("snd_pcm_hw_params_set_format failed:");
//      exit(1);
//    }
//    rc=snd_pcm_hw_params_set_channels(handle, params, channels); //设置声道,1表示单声>道，2表示立体声
//    if(rc<0){
//      perror("\nsnd_pcm_hw_params_set_channels:");
//      exit(1);
//    }
//    rc=snd_pcm_hw_params_set_rate_near(handle, params, &rate, &dir); //设置>频率
//    if(rc<0){
//      perror("\nsnd_pcm_hw_params_set_rate_near:");
//      exit(1);
//    }
//    rc = snd_pcm_hw_params(handle, params);
//    if(rc<0)    {
//      perror("\nsnd_pcm_hw_params: ");
//      exit(1);
//    }
//    return 0;
//}


//void set_volume(long volume)
//{
//    snd_mixer_t *mixerFd;
//    snd_mixer_elem_t *elem;
//    long minVolume = 0,maxVolume = 100;
//    int result;
//    // 打开混音器
//    if ((result = snd_mixer_open( &mixerFd, 0)) < 0){
//        printf("snd_mixer_open error!\n");
//        mixerFd = NULL;
//    }
//    // Attach an HCTL to an opened mixer
//    if ((result = snd_mixer_attach( mixerFd, "default")) < 0){
//        printf("snd_mixer_attach error!\n");
//        snd_mixer_close(mixerFd);
//        mixerFd = NULL;
//    }
//    // 注册混音器
//    if ((result = snd_mixer_selem_register( mixerFd, NULL, NULL)) < 0){
//        printf("snd_mixer_selem_register error!\n");
//        snd_mixer_close(mixerFd);
//        mixerFd = NULL;
//    }
//    // 加载混音器
//    if ((result = snd_mixer_load( mixerFd)) < 0){
//        printf("snd_mixer_load error!\n");
//        snd_mixer_close(mixerFd);
//        mixerFd = NULL;
//    }

//    // 遍历混音器元素
//    for(elem=snd_mixer_first_elem(mixerFd); elem; elem=snd_mixer_elem_next(elem)){
//        if (snd_mixer_elem_get_type(elem) == SND_MIXER_ELEM_SIMPLE &&
//            snd_mixer_selem_is_active(elem)){ // 找到可以用的, 激活的elem
//            snd_mixer_selem_get_playback_volume_range(elem, &minVolume, &maxVolume);
//            snd_mixer_selem_set_playback_volume_all(elem, volume);
//        }
//    }
//    snd_mixer_close(mixerFd);
//}

////libmad设置相关函数
///*
// * This is a private message structure. A generic pointer to this structure
// * is passed to each of the callback functions. Put here any data you need
// * to access from within the callbacks.
// */
//struct buffer {
//  unsigned char const *start;
//  unsigned long length;
//};

///*
// * This is the input callback. The purpose of this callback is to (re)fill
// * the stream buffer which is to be decoded. In this example, an entire file
// * has been mapped into memory, so we just call mad_stream_buffer() with the
// * address and length of the mapping. When this callback is called a second
// * time, we are finished decoding.
// */

//static enum mad_flow input(void *data,struct mad_stream *stream)
//{
//  struct buffer *buffer = (struct buffer *)data;
//  if (!buffer->length)
//    return MAD_FLOW_STOP;

//  mad_stream_buffer(stream, buffer->start, buffer->length);
//  buffer->length = 0;
//  return MAD_FLOW_CONTINUE;
//}

///*
// * The following utility routine performs simple rounding, clipping, and
// * scaling of MAD's high-resolution samples down to 16 bits. It does not
// * perform any dithering or noise shaping, which would be recommended to
// * obtain any exceptional audio quality. It is therefore not recommended to
// * use this routine if high-quality output is desired.
// */

//static inline signed int scale(mad_fixed_t sample)
//{
//  /* round */
//  sample += (1L << (MAD_F_FRACBITS - 16));

//  /* clip */
//  if (sample >= MAD_F_ONE)
//    sample = MAD_F_ONE - 1;
//  else if (sample < -MAD_F_ONE)
//    sample = -MAD_F_ONE;

//  /* quantize */
//  return sample >> (MAD_F_FRACBITS + 1 - 16);
//}

///*
// * This is the output callback function. It is called after each frame of
// * MPEG audio data has been completely decoded. The purpose of this callback
// * is to output (or play) the decoded PCM audio.
// */

//static enum mad_flow output(void *data,struct mad_header const *header,struct mad_pcm *pcm)
//{
//    unsigned int nchannels, nsamples,n;
//    mad_fixed_t const *left_ch, *right_ch;

//    /* pcm->samplerate contains the sampling frequency */
//    nchannels = pcm->channels;
//    n=nsamples = pcm->length;
//    left_ch = pcm->samples[0];
//    right_ch = pcm->samples[1];

//    unsigned char Output[6912], *OutputPtr;
//    int fmt, wrote, speed, exact_rate, err, dir;

//    OutputPtr = Output;

//    while (nsamples--){
//        signed int sample;

//        /* output sample(s) in 16-bit signed little-endian PCM */
//        sample = scale(*left_ch++);

//        *(OutputPtr++) = sample >> 0;
//        *(OutputPtr++) = sample >> 8;
//        if (nchannels == 2){
//            sample = scale (*right_ch++);
//            *(OutputPtr++) = sample >> 0;
//            *(OutputPtr++) = sample >> 8;
//        }
//    }
//    OutputPtr = Output;
//    snd_pcm_writei (handle, OutputPtr, n);
//    OutputPtr = Output;

//    return MAD_FLOW_CONTINUE;
//}

///*
// * This is the error callback function. It is called whenever a decoding
// * error occurs. The error is indicated by stream->error; the list of
// * possible MAD_ERROR_* errors can be found in the mad.h (or stream.h)
// * header file.
// */

//static enum mad_flow error(void *data,struct mad_stream *stream,struct mad_frame *frame)
//{
//    struct buffer *buffer = (struct buffer *)data;
//    printf("this is mad_flow error\n");
//    fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
//    stream->error, mad_stream_errorstr(stream),
//    stream->this_frame - buffer->start);

//    /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */
//    return MAD_FLOW_CONTINUE;
//}

///*
// * This is the function called by main() above to perform all the decoding.
// * It instantiates a decoder object and configures it with the input,
// * output, and error callback functions above. A single call to
// * mad_decoder_run() continues until a callback function returns
// * MAD_FLOW_STOP (to stop decoding) or MAD_FLOW_BREAK (to stop decoding and
// * signal an error).
// */
//static int decode(unsigned char const *start, unsigned long length){
//  struct buffer buffer;
//  struct mad_decoder decoder;
//  int result;

//  /* initialize our private message structure */
//  buffer.start = start;
//  buffer.length = length;

//  /* configure input, output, and error functions */
//  mad_decoder_init(&decoder, &buffer,
//           input, 0 /* header */, 0 /* filter */, output,
//           error, 0 /* message */);

//  /* start decoding */
//  result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

//  /* release the decoder */
//  mad_decoder_finish(&decoder);

//  return result;
//}

////home/yk/Desktop/MP3/mp3/playmp3.cpp:416: error: undefined reference to `mad_stream_buffer'

