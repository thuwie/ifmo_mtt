package edu.ifmo.konovalov;

import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.sql.Dataset;
import org.apache.spark.sql.Row;
import org.apache.spark.sql.SparkSession;


import java.util.Objects;

import static org.apache.spark.sql.functions.*;

public class MapReduce {
    public static final String port = ":9000";
    private static String hdfsHost;

    public static void main(String[] args) {
        hdfsHost = "hdfs://" + args[0] + port;
        SparkSession sparkSession = SparkSession.builder().master("local").appName("MapReduce").getOrCreate();

        JavaRDD<LogObject> data = sparkSession.read().textFile(hdfsHost + args[1] + "/access_logs")
                .javaRDD()
                .map(LogObject::getObjectFromLine)
                .filter(Objects::nonNull);

        Dataset<Row> dataSet = sparkSession.createDataFrame(data, LogObject.class);

        taskOne(dataSet);
        taskTwo(dataSet);
        taskThree(dataSet);
    }

    /**
     * Подготовить список запросов, которые закончились 5xx ошибкой, с количеством
     * неудачных запросов
     */
    private static void taskOne(Dataset<Row> data) {
        data.filter(col("httpResponseCode").between(500, 599))
                .groupBy("httpEndpoint")
                .count()
                .select("httpEndpoint", "count")
                .coalesce(1)
                .toJavaRDD()
                .saveAsTextFile(hdfsHost + "/taskOne");
    }

    /**
     * Подготовить временной ряд с количеством запросов по датам для всех
     * используемых комбинаций http методов и return codes. Исключить из
     * результирующего файла комбинации, где количество событий в сумме было
     * меньше 10.
     *
     * @param dataSet
     */
    private static void taskTwo(Dataset<Row> dataSet) {
        dataSet.groupBy("httpMethod", "httpResponseCode", "httpTimestamp")
                .count()
                .filter(col("count").geq(10))
                .select("httpTimestamp", "httpMethod", "httpResponseCode", "count")
                .sort("httpTimestamp", "httpMethod", "httpResponseCode")
                .coalesce(1)
                .toJavaRDD()
                .saveAsTextFile(hdfsHost + "/taskTwo");
    }

    /**
     * Произвести расчет скользящим окном в одну неделю количества запросов
     * закончившихся с кодами 4xx и 5xx
     *
     * @param dataSet
     */
    private static void taskThree(Dataset<Row> dataSet) {
        dataSet.filter(col("httpResponseCode").between(400, 599))
                .groupBy(window(to_date(col("httpTimestamp"), "dd/MM/yyyy"), "1 week", "1 day"))
                .count()
                .select(date_format(col("window.start"), "dd/MM/yyyy"),
                        date_format(col("window.end"), "dd/MM/yyyy"),
                        col("count"))
                .coalesce(1)
                .toJavaRDD()
                .saveAsTextFile(hdfsHost + "/taskThree");
    }
}



