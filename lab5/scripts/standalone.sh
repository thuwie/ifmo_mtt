#!/bin/bash

sh start.sh

spark-submit --master spark://master:7077 \
             --class edu.ifmo.konovalov.MapReduce /target/spark-1.0.jar master /logs