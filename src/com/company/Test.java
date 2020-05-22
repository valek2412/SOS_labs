package com.company;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class Test {
    public static void main(String[] args) throws IOException {
        double lamda1 = 10;
        int lamda2 = 5;
        BufferedWriter file1 = new BufferedWriter(new FileWriter(new File("Graphic_1.csv")));
        BufferedWriter file2 = new BufferedWriter(new FileWriter(new File("Graphic_2.csv")));
        BufferedWriter file3 = new BufferedWriter(new FileWriter(new File("Graphic_3.csv")));
        Generator generator = new Generator();
        for(lamda1 = 1; lamda1 <= 10; lamda1+=.1){
            int globalTime = 0;
            int timeNext = 0;
            int id = 1;
            int avarageTime = 0;
            Processor proc = new Processor();
            for( globalTime =0; globalTime < 10000; globalTime++){
                if(timeNext <= 0){
                    timeNext = generator.generateExponential(lamda1);
                    proc.addRequest(id, lamda2, globalTime);
                    id++;
                }
                proc.procesRequest(globalTime);
                timeNext--;
            }

            avarageTime = proc.getAvarageTime();
            lamda1 *= 10;
            lamda1 = Math.round(lamda1);
            lamda1 /= 10;
            file1.write(Double.toString(lamda1)+";"+avarageTime+"\n");
            file2.write(Double.toString(lamda1)+";"+proc.getStopTime()+"\n");
            if(lamda1 == 3.0){
                proc.writeStatictic(lamda1);
                for (int i = 0; i < proc.getTimePriority().length; i++){
                    file3.write(Integer.toString(i+1)+";"+Integer.toString(proc.getTimePriority()[i])+"\n");
                }
            }
        }
        file1.close();
        file2.close();
        file3.close();
    }
}