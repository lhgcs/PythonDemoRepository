#include "xmldom.h"
#include "util/firmwaredownload.h"
#include "util/filehandler.h"
int XmlDom::DisplayMode = DISPLAY_MODE_HORIZONTAL;
QString XmlDom::ip = "http://hzgzj.veiding.com";
QString XmlDom::port = "80";
QString XmlDom::tcp_ip = "112.74.59.162";
QString XmlDom::tcp_port = "54991";
QString XmlDom::id = "201712260587012";
QString XmlDom::image_path = "/home/image/";
QString XmlDom::video_path = "/home/video/";

int XmlDom::screen_width = 1280;
int XmlDom::screen_height = 720;

int XmlDom::RegionSize = 2;
QSize XmlDom::RegionInfo[MAX_DISPLAY_REGION_SIZE] = {QSize(1280, 320),QSize(1280, 350),QSize(1280, 420)};
int XmlDom::ChnagePlayMode  = PLAY_MODE_UNKNOW;
int XmlDom::CurrentPlayMode = PLAY_MODE_UNKNOW;

//QString XmlDom::ico_path = "/home/ico/";


QString XmlDom::price="10.0";

XmlDom::XmlDom()
{
}


QDomElement XmlDom::add_child_domElement(QDomDocument &doc,QDomElement &root,QString key){
    QDomElement element = doc.createElement(key);//创建对应的节点  QString::fromLocal8Bit(key)
    root.appendChild(element); //元素

    return element;
}

void XmlDom::add_child_domElement_attribute(QDomDocument &doc,QDomElement &root, const QString attr_key, const QString attr_value,
                          const QString key,const QString value)
{

    QDomElement element = doc.createElement(key);//创建对应的节点

    QDomAttr Attr = doc.createAttribute(attr_key);  //创建属性
    Attr.setNodeValue(attr_value);                  //设置属性
    element.setAttributeNode(Attr);

    QDomText text = doc.createTextNode(value);
    root.appendChild(element);//元素
    element.appendChild(text);//元素的文本数据
}

void XmlDom::add_child_domElement(QDomDocument &doc,QDomElement &root,
                          const QString key,const QString value)
{
    QDomElement element = doc.createElement(key);//创建对应的节点
    QDomText text = doc.createTextNode(value);
    root.appendChild(element);//元素
    element.appendChild(text);//元素的文本数据
}


void XmlDom::save_file(const QString fileName,const QDomDocument &doc){
    // 保存 XML 文件
    QString strFile(fileName);
    QFile file(strFile);
    if(file.open(QFile::WriteOnly | QFile::Text)) { // 只写模式打开文件
        QTextStream out(&file);
        doc.save(out, QDomNode::EncodingFromDocument);
        file.close();
    }
}



bool XmlDom::read_file(const QString fileName,QDomDocument &doc){
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    if (!doc.setContent(&file)) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

void XmlDom::parserConfig(QDomNode node){

    QDomElement element = node.toElement();  //尝试将节点转换为元素
    if (!element.isNull()) {                 //节点的确是一个元素

        qDebug() << element.tagName();
        /**********遍历元素**********/
        QDomNodeList list = element.childNodes();
        for (int i = 0; i < list.count(); i++) {
            node = list.at(i);

            if (node.isElement()) {
                element = node.toElement();
                qDebug() << element.tagName() << element.text();//tagName() 返回标签的名称，text() 则返回相应的内容
                QString key=element.tagName();

                //http
                if(!QString::compare(key,QStringLiteral("ip"))){
                    ip=element.text();
                }
                else if(!QString::compare(key,QStringLiteral("port"))) {
                    port=element.text();
                }

                //tcp
                if(!QString::compare(key,QStringLiteral("tcp_ip"))){
                    tcp_ip=element.text();
                }
                else if(!QString::compare(key,QStringLiteral("tcp_port"))) {
                    tcp_port=element.text();
                }

                //device id
                else if (!QString::compare(key,QStringLiteral("id"))) {
                    id=element.text();
                }
//                else if (!QString::compare(key,QStringLiteral("tcp_id"))) {
//                    tcp_id=element.text();
//                }

                //image
                else if (!QString::compare(key,QStringLiteral("image_path"))) {
                    image_path=element.text();
                }
                else if (!QString::compare(key,QStringLiteral("video_path"))) {
                    video_path=element.text();
                }

                //region
                else if (!QString::compare(key,QStringLiteral("region_size"))) {
                    RegionSize=element.text().toInt();
                }
                else if (!QString::compare(key,QStringLiteral("region_info_width"))) {
                    QString value ,Width;
                    value = element.attribute("id");
                    qDebug() <<value;
                    Width = element.text();
                    RegionInfo[value.toInt()].setWidth(Width.toInt());

                }
                else if (!QString::compare(key,QStringLiteral("region_info_height"))) {
                    QString value ,Height;
                    value = element.attribute("id");
                    qDebug() <<value;

                    Height = element.text();
                    RegionInfo[value.toInt()].setHeight(Height.toInt());

                }
            }
        }
    }
}



/*#####################################public#########################################################*/
void XmlDom::writeConfigXml(QString fileName) {
    QDomDocument doc;// 构造一个 DOM 文档
    QDomProcessingInstruction xmlInstruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
    QDomComment comment = doc.createComment(QString::fromLocal8Bit("System config"));
//    QDomProcessingInstruction styleInstruction = doc.createProcessingInstruction("xml-stylesheet", "type=\"text/css\" href=\"style.css\"");
    doc.appendChild(xmlInstruction);    // 开始文档（XML声明）
    doc.appendChild(comment);           // 注释
//    doc.appendChild(styleInstruction);  // 处理指令
    // 根元素 <root>
    QDomElement root = doc.createElement("root");
    root.setAttribute("Version", FirmWareDownload::getSoftWareVersion());          // 属性
    doc.appendChild(root);

    //network
    QDomElement child=add_child_domElement(doc,root,"NetWork");
    add_child_domElement(doc,child,"ip", ip);
    add_child_domElement(doc,child,"port", port);
    add_child_domElement(doc,child,"tcp_ip", tcp_ip);
    add_child_domElement(doc,child,"tcp_port", tcp_port);

    //mac id
    QDomElement childId=add_child_domElement(doc,root,"Device");
    add_child_domElement(doc,childId,"id",id);
//    add_child_domElement(doc,childId,"tcp_id",tcp_id);

    //resouce
    QDomElement childResouce=add_child_domElement(doc,root,"Resource");
    add_child_domElement(doc,childResouce,"image_path",image_path);
    add_child_domElement(doc,childResouce,"video_path",video_path);

    //Region
    QDomElement childRegion=add_child_domElement(doc,root,"Region");
    add_child_domElement(doc,childRegion,"region_size",QString::number(RegionSize));

    for(int i = 0; i<RegionSize; i++)
    {
        add_child_domElement_attribute(doc,childRegion, "id", QString::number(i), "region_info_width" , QString::number(RegionInfo[i].width()));
        add_child_domElement_attribute(doc,childRegion, "id", QString::number(i), "region_info_height" , QString::number(RegionInfo[i].height()));
    }



    save_file(fileName,doc);
}

bool XmlDom::checkConfigXmlVersion(QDomElement root) {

    if (root.hasAttribute("Version"))
    {
       if(root.attribute("Version") == FirmWareDownload::getSoftWareVersion())
       {
           qDebug()<<"config file is  same version";
            return true;
       }
       else
       {
            qDebug()<<"config file is not same version! old verison :"<<root.attribute("Version")<<" new version :"<<FirmWareDownload::getSoftWareVersion();
       }
    }
    else
    {
        qDebug()<<"not has verison attribute";
    }

    return false;
}

bool XmlDom::initConfigXml()
{
    QDomElement root;
    QDomDocument doc;
    QString fileName ="/home/conf/conf.xml";
    //系统参数配置
    if((!is_file_exits(fileName)))
    {
        qDebug()<<"file not exits wirte config";
        //配置文件不存在
        writeConfigXml(fileName);
    }
    else
    {
        if(!read_file(fileName,doc)){
            return false;
        }

        root = doc.documentElement();//根元素


        //检查配置文件版本号
        if(checkConfigXmlVersion(root))
        {
            qDebug()<<"read config file";
           //读取配置文件
           readConfigXml(root);
        }
        else
        {
            qDebug()<<"rewrite config file";
           //版本号不对重新写入配置文件
           writeConfigXml("/home/conf/conf.xml");
        }
    }

    return true;
}

bool XmlDom::readConfigXml(QDomElement root) {

    qDebug() << "\n########################SystemConfigParser Satrt########################\n";

    QDomNode node = root.previousSibling();  //向上查找兄弟节点
    while (!node.isNull()) {
        switch (node.nodeType()) {
            case QDomNode::ProcessingInstructionNode : {
                QDomProcessingInstruction instruction = node.toProcessingInstruction();
                qDebug() << instruction.target() << instruction.data();
                if(QString::compare(instruction.target(), "xml") == 0) {                   // 开始文档（XML 声明）
                }
                else if(QString::compare(instruction.target(), "xml-stylesheet") == 0) { // 处理指令
                }
                break;
            }
            case QDomNode::CommentNode : {
                QDomComment comment = node.toComment();
                qDebug()<< comment.data();//
                break;
            }
            default:break;
        }
        node = node.previousSibling();
    }

    /**********元素 <network>**********/
    node = root.firstChild(); //返回根节点的第一个子节点

    while (!node.isNull())    {
        if(node.isElement()) {
           parserConfig(node);
        }
        node=node.nextSibling();//向下查找
    }

     qDebug() << "\n########################SystemConfigParser End########################\n";

    if(ip.isEmpty()||port.isEmpty()||id.isEmpty()){
        return false;
    }
    return true;
}


bool XmlDom::readConfigXml(QString fileName) {
    QDomDocument doc;
    if(!read_file(fileName,doc)){
        return false;
    }
    /**********根元素 <root>**********/
    QDomElement root = doc.documentElement();//根元素

    qDebug() << "\n########################SystemConfigParser Satrt########################\n";
//    if (root.hasAttribute("Version"))        // 属性
//        qDebug() << root.attribute("Version");
    /**********根元素之上（XML 声明、注释等）**********/

    QDomNode node = root.previousSibling();  //向上查找兄弟节点
    while (!node.isNull()) {
        switch (node.nodeType()) {
            case QDomNode::ProcessingInstructionNode : {
                QDomProcessingInstruction instruction = node.toProcessingInstruction();
                qDebug() << instruction.target() << instruction.data();
                if(QString::compare(instruction.target(), "xml") == 0) {                   // 开始文档（XML 声明）
                }
                else if(QString::compare(instruction.target(), "xml-stylesheet") == 0) { // 处理指令
                }
                break;
            }
            case QDomNode::CommentNode : {
                QDomComment comment = node.toComment();
                qDebug()<< comment.data();//
                break;
            }
            default:break;
        }
        node = node.previousSibling();
    }

    /**********元素 <network>**********/
    node = root.firstChild(); //返回根节点的第一个子节点

    while (!node.isNull())    {
        if(node.isElement()) {
           parserConfig(node);
        }
        node=node.nextSibling();//向下查找
    }

     qDebug() << "\n########################SystemConfigParser End########################\n";

    if(ip.isEmpty()||port.isEmpty()||id.isEmpty()){
        return false;
    }
    return true;
}
