FROM gettyimages/spark:2.4.1-hadoop-3.0

# Set JAVA_HOME
ENV JAVA_HOME /usr/lib/jvm/java-8-openjdk-amd64

# Update and install comands
RUN apt-get -y update && \
	apt-get -y upgrade && \
	apt-get -y install ssh wget openssh-server

RUN ssh-keygen -t rsa -f ~/.ssh/id_rsa -P '' && \
   cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys && \
   echo "HOST *\n	StrictHostKeyChecking no\n	UserKnownHostsFile=/dev/null" > $HOME/.ssh/config

RUN mkdir -p ~/home/logs

RUN wget ftp://ita.ee.lbl.gov/traces/NASA_access_log_Jul95.gz && \
	gunzip -c NASA_access_log_Jul95.gz > ~/home/access_logs



COPY hadoop/* $HADOOP_CONF_DIR/
RUN chmod u+x $HADOOP_CONF_DIR/hadoop-env.sh
COPY scripts/* /scripts/

RUN mkdir -p /app/hadoop/tmp && \
   mkdir -p /usr/local/hadoop_store/hdfs/namenode && \
   mkdir -p /usr/local/hadoop_store/hdfs/datanode
RUN hdfs namenode -format

ENTRYPOINT [ "/bin/bash", "-c", "service ssh start; tail -f /dev/null"]