#!/bin/bash

stop-yarn.sh

hadoop fs -rm -r -f /taskOne /taskTwo /taskThree

stop-dfs.sh

sed -i s/$HOSTNAME/master/ $HADOOP_CONF_DIR/core-site.xml