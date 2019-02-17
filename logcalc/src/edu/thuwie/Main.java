package edu.thuwie;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Main {

    public static void main(String[] args) {
        final long startTime = System.currentTimeMillis();
        int stepsCount = 100000000;
        int jobsCount = Integer.parseInt(args[0]);
        double A = 0.0;
        double B = 1.0;
        double rez = 0.0;
        double length = (B - A) / stepsCount;
        List<Future<Double>> results = new ArrayList<>();
        System.out.println(jobsCount);
        ExecutorService executor = Executors.newFixedThreadPool(jobsCount);

        for (int i = 0; i < jobsCount; i++) {
            CalcJob calcJob = new CalcJob(A + (B / jobsCount * i), length, stepsCount / jobsCount, i);
            final Future<Double> result = executor.submit(calcJob);
            results.add(result);
        }

        for (Future<Double> res : results) {
            try {
                rez = rez + res.get();
            } catch (InterruptedException | ExecutionException ex) {
                System.out.println("Exception: " + ex.getMessage());
            }
        }
        executor.shutdown();
        final long endTime = System.currentTimeMillis();

        System.out.println("Total execution time: " + (endTime - startTime));

//        System.out.println(rez);
    }
}
