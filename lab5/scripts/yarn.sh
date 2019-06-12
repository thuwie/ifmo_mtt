#!/bin/bash

sh start.sh

start-yarn.sh

spark-submit --master yarn \
             --deploy-mode client \
             --class edu.ifmo.konovalov.MapReduce /target/spark-1.0.jar master /logs