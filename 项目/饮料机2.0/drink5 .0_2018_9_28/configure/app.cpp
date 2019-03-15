#include "app.h"
#include "subwidget/dialoginstance.h"
#include "configure/toast.h"

int App::screenWidget=720;
int App::screenHeight=1280;

QString App::HTTP="127.0.0.1";
QString App::MACID="0";

QString App::dataBaseName="/home/sql/database.db";
QString App::goodsTableName="t";

int App::goodsTotal=42;
int App::payType=3;

App::App(){
}

/**
  检查系统更新
 * @brief MainWindow::check_system_version
 */
void App::check_system_version(){

    QString url;
    #if NEW_DRINK
    url = App::HTTP + "/update_apk/check.php";
    #elif JIUHAOCANG
    url=App::HTTP+"/update_version/newbottle/check.php";
    #else
    url=App::HTTP+"/update_version/check.php";
    #endif

    QString version_new;
    FirmWareDownload firmWareDownload;
    firmWareDownload.checkVersion(url,version_new);
    if(firmWareDownload.isUpperServerVerison()){
        Toast::getInstance()->set_text("开始下载固件");
        if(firmWareDownload.startDownloadFirmWare(version_new)){
            Toast::getInstance()->set_text("固件下载成功,系统将重启");
            QCoreApplication::processEvents();
            sleep(3);
            system("reboot");
        }else{
            Toast::getInstance()->set_text("固件下载失败");
        }
    }
}
