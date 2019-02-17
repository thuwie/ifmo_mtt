package edu.thuwie;

import java.util.concurrent.Callable;

public class CalcJob implements Callable {
    private Thread t;
    private double result = 0.0;
    private double A, length;
    private int steps;
    private String name;

    public CalcJob(double A, double length, int stepsCount, int num) {
        this.A = A;
        this.steps = stepsCount;
        this.length = length;
        this.name = String.valueOf(num);
    }

    @Override
    public Object call() throws Exception {
//        System.out.println("Node [" + name + "] consumed A: " + this.A);
        for (int i = 0; i < steps; i++) {
            result += calc(0.5 * (this.A + (i * length) + this.A + ((i + 1) * length))) * length;
//            System.out.println("Node [" + name + "] + step [" + i + "] produced: " + result);

        }
//        System.out.println("Node [" + name + "] produced: " + result);
        return result;
    }

    private double calc(double x) {
        return x;
    }
}
