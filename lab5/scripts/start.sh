#!/bin/bash

chmod +x /target/spark-1.0.jar

sed -i s/master/$HOSTNAME/ $HADOOP_CONF_DIR/core-site.xml

${HADOOP_HOME}/sbin/start-all.sh

hadoop fs -mkdir /logs
hadoop fs -put ~/home/access_logs /logs