#ifndef XMLDOM_H
#define XMLDOM_H

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "filehandler.h"
#include "app.h"

//#define name2str(name) (#name)

class XmlDom
{
public:
    XmlDom();

    void writeXML(QString fileName) {
        QDomDocument doc;// 构造一个 DOM 文档
        QDomProcessingInstruction xmlInstruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
        QDomComment comment = doc.createComment(QString::fromLocal8Bit("系统配置"));
        QDomProcessingInstruction styleInstruction = doc.createProcessingInstruction("xml-stylesheet", "type=\"text/css\" href=\"style.css\"");
        doc.appendChild(xmlInstruction);    // 开始文档（XML声明）
        doc.appendChild(comment);           // 注释
        doc.appendChild(styleInstruction);  // 处理指令
        // 根元素 <root>
        QDomElement root = doc.createElement("root");
        root.setAttribute("Version", "2.0");          // 属性
        doc.appendChild(root);
        // 元素 <networt>   ---->  元素 <IP>、<PORT>
        QDomElement child=add_child_domElement(doc,root,"NETWORT");
        add_child_domElement(doc,child,"ip",App::ip);
        add_child_domElement(doc,child,"port",App::port);

        QDomElement childId=add_child_domElement(doc,root,"ID");
        add_child_domElement(doc,childId,"id",App::macId);

        QDomElement childUser=add_child_domElement(doc,root,"USER");
        add_child_domElement(doc,childUser,"user",App::user);
        add_child_domElement(doc,childUser,"password",App::password);

        QDomElement childScreen=add_child_domElement(doc,root,"SCREEN");
        add_child_domElement(doc,childScreen,"width",QString::number(App::DeskWidth));
        add_child_domElement(doc,childScreen,"height",QString::number(App::DeskHeight));

        QDomElement childVisual=add_child_domElement(doc,root,"VISUAL");
        for(int i=0;i<App::deviceNum;i++){
            if(App::deviceVisual[i]){
                add_child_domElement(doc,childVisual,QString(i+65),"1");
            }
            else{
                add_child_domElement(doc,childVisual,QString(i+65),"0");
            }
        }

        save_file(fileName,doc);
    }

    bool readXML(QString fileName) {       
        QDomDocument doc;
        if(!read_file(fileName,doc)){
            qDebug()<<"read conf file fail";
            return false;
        }
        /**********根元素 <root>**********/
        QDomElement root = doc.documentElement();//根元素
        qDebug() << root.tagName();
        if (root.hasAttribute("Version"))        // 属性
        qDebug() << root.attribute("Version");
        /**********根元素之上（XML 声明、注释等）**********/
        QDomNode node = root.previousSibling();  //向上查找兄弟节点
        while (!node.isNull()) {
            switch (node.nodeType()) {
                case QDomNode::ProcessingInstructionNode : {
                    QDomProcessingInstruction instruction = node.toProcessingInstruction();
//                    qDebug() <<"1"<< instruction.target() << instruction.data();
                    if(QString::compare(instruction.target(), "xml") == 0) {                 // 开始文档XML声明）
//                        qDebug()<<"XML声明";
                    }
                    else if(QString::compare(instruction.target(), "xml-stylesheet") == 0) { // 处理指令
//                        qDebug()<<"处理指令";
                    }
                    break;
                }
                case QDomNode::CommentNode : {
                    QDomComment comment = node.toComment();
//                    qDebug()<<"2"<< comment.data();
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
               traversal(node);
            }
            node=node.nextSibling();//向下查找
        }

        if(App::ip.isEmpty()||App::port.isEmpty()||App::macId.isEmpty()){
            qDebug()<<"file NULL";
            return false;
        }
        return true;
    }
private:
    QDomElement add_child_domElement(QDomDocument &doc,QDomElement &root,QString key){
        QDomElement element = doc.createElement(key);//创建对应的节点  QString::fromLocal8Bit(key)
        root.appendChild(element);                   //元素
        return element;
    }
    void add_child_domElement(QDomDocument &doc,QDomElement &root,const QString key,const QString value){
        QDomElement element = doc.createElement(key);//创建对应的节点
        QDomText text = doc.createTextNode(value);
        root.appendChild(element);                   //元素
        element.appendChild(text);                   //元素的文本数据
    }
    void save_file(const QString fileName,const QDomDocument &doc){// 保存 XML 文件
        QString strFile(fileName);
        QFile file(strFile);
        if(file.open(QFile::WriteOnly | QFile::Text)) {            // 只写模式打开文件
            QTextStream out(&file);
            doc.save(out, QDomNode::EncodingFromDocument);
            file.close();
            system("sync");
        }
    }
    bool read_file(const QString fileName,QDomDocument &doc){
        QFile file(fileName);
        if(!is_file_exits(fileName)){                             //准确判断文件是否存在
            file.open(QIODevice::WriteOnly);
            file.close();
            writeXML(fileName);
            qDebug()<<"not file:"<<fileName;
            return false;
        }
        if(!file.open(QIODevice::ReadOnly)){
            qDebug()<<"open file fail:"<<fileName;
            return false;
        }
            //setContent是将指定的内容指定给QDomDocument解析，
            //第一参数可以是QByteArray或者是文件名等
            QString errorStr;
            int errorLine;
            int errorCol;
            if(!doc.setContent(&file,true,&errorStr,&errorLine,&errorCol)){
                //如果出错，则会进入这里。errorStr得到的是出错说明
                //errorLine和errorCol则是出错的行和列
                qDebug() <<"error:"<< errorStr << "line: " << errorLine << "col: " << errorCol;
                file.close();
                file.remove();
                writeXML(fileName);
                return false;
            }

//        if (!doc.setContent(&file)) {
//            file.close();
//            return false;
//        }
        file.close();
        return true;
    }

    void traversal(QDomNode node){
        QDomElement element = node.toElement();  //尝试将节点转换为元素
        if (!element.isNull()) {                 //节点的确是一个元素
            /**********遍历元素**********/
            QDomNodeList list = element.childNodes();
            for (int i = 0; i < list.count(); i++) {
                node = list.at(i);
                if (node.isElement()) {
                    element = node.toElement();
                    //qDebug() << element.tagName() << element.text();//tagName() 返回标签的名称，text() 则返回相应的内容
                    QString key=element.tagName();
                    if(!QString::compare(key,QStringLiteral("ip"))){
                        App::ip=element.text();
                    }
                    else if(!QString::compare(key,QStringLiteral("port"))) {
                        //port=element.text();
                    }
                    else if (!QString::compare(key,QStringLiteral("id"))) {
                        App::macId=element.text();
                    }
                    else if(!QString::compare(key,QStringLiteral("user"))) {
                        App::user=element.text();
                    }
                    else if (!QString::compare(key,QStringLiteral("password"))) {
                        App::password=element.text();
                    }
                    else if(!QString::compare(key,QStringLiteral("width"))) {
                        App::DeskWidth=element.text().toInt();
                    }
                    else if (!QString::compare(key,QStringLiteral("height"))) {
                        App::DeskHeight=element.text().toInt();
                    }
                    else if (!QString::compare(key,QStringLiteral("A"))) {
                        if(!element.text().compare("0")){
                            App::deviceVisual[0]=false;
                        }
                        else{
                            App::deviceVisual[0]=true;
                        }
                    }
                    else if (!QString::compare(key,QStringLiteral("B"))) {
                        if(!element.text().compare("0")){
                            App::deviceVisual[1]=false;
                        }
                        else{
                            App::deviceVisual[1]=true;
                        }
                    }
                    else if (!QString::compare(key,QStringLiteral("C"))) {
                        if(!element.text().compare("0")){
                            App::deviceVisual[2]=false;
                        }
                        else{
                            App::deviceVisual[2]=true;
                        }
                    }
                    else if (!QString::compare(key,QStringLiteral("D"))) {
                        if(!element.text().compare("0")){
                            App::deviceVisual[3]=false;
                        }
                        else{
                            App::deviceVisual[3]=true;
                        }
                    }
                    else if (!QString::compare(key,QStringLiteral("E"))) {
                        if(!element.text().compare("0")){
                            App::deviceVisual[4]=false;
                        }
                        else{
                            App::deviceVisual[4]=true;
                        }
                    }

                }
            }
        }
    }
};

#endif // XMLDOM_H
