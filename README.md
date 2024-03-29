# 概述
此软件主要分为四个模块
* 数据查询
* 告警配置
* 软件设置
* 帮助

## 数据查询
* 查看实时数据
  您可以查看远程数据中心的实时数据，你可以设定分组ID和设备编号来唯一定位一台设备，由于数据的查询为多线程运行，因查询语句不会阻塞图形界面。但同时过快的查询也是没有必要的，因此设计的最快的刷新时间为100ms，当用户设定的值小于100ms时，将被重设为100ms
* 编辑SQL指令
  您可以在该面板中编写SQL指令并执行，查询结果可以实时显示或将保存到指定文件中，查询依然实在启动另外一个线程中运行的
* 打开SQL脚本
  您可以打开自己编写的SQL脚本
* 登录Grafana
将使用浏览器打开用户配置的Grafana地址

## 告警配置
* 显示告警规则
此功能将通过HTTP协议从远程服务器获取规则，默认以原格式显示，您可以点击“缩进显示JSON”来对规则进行格式化显示，方便阅读。同时点击“删除规则”时可以写入规则名称并进行删除
* 配置告警信息
您可以通过UI界面来配置告警信息，并点击上传到服务器，界面里提供了配置说明
* 打开AlertManager
将使用默认浏览器打开用户配置的AlertManager地址

## 软件设置
* 偏好设置
  包含系统设置、登录设置、数据库设置、Grafana设置和AlertManager设置，这些设置在任何平台都将存储到ini文件中

* 清空主界面
  将关闭已经打开的主界面

## 帮助
* 查看运行日志
  如果软件出现问题，您可以查看日志来定位问题
* 查看使用文档
  您可以看到本文档
* 关于软件
  显示软件信息