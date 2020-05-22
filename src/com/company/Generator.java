package com.company;

public class Generator {
    public int generateExponential(double lambda) {
        return (int)(-100*Math.log(1-Math.random())/lambda);
    }
}