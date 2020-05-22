package com.company;

import com.company.Generator;
import com.company.Request;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class Processor {
    ArrayList<ArrayList<Request>> quenes = new ArrayList<ArrayList<Request>>();

    ArrayList<Request> readyRequest = new ArrayList<Request>();

    boolean procStatus = true;

    Request r;
    int stopTime;
    boolean stopStatus = true;

    int Prioruty_max = 11;

    int[] timePriority = new int[Prioruty_max];
    int[] counter = new int[Prioruty_max];

    public void addRequest(int id, int lamda, int time){
        Generator generator = new Generator();
        int timeService = 0;
        while(true){
            if(timeService < 1){
                timeService = generator.generateExponential(lamda);
            } else{
                break;
            }
        }
        int priority = 0;
        while(true){
            if(priority > 11 || priority < 1){
                priority = generator.generateExponential(lamda);
            } else{
                break;
            }
        }
        Request request = new Request(priority, id, timeService, time);
        boolean status = false;
        for(int i = 0; i < quenes.size(); i++){
            if(quenes.get(i).get(0).getPriority() == request.getPriority()){
                quenes.get(i).add(request);
                status = true;
            }
        }
        if(status){
            return;
        } else {
            addNewLine(request);
        }
    }
    public void addNewLine(Request request){
        quenes.add(new ArrayList<Request>());
        quenes.get(quenes.size()-1).add(request);
    }
    public void procesRequest(int time){
        int maxPriority = 0;
        int queneMax = 0;
        if(procStatus && !quenes.isEmpty()){
            for(int i = 0; i < quenes.size(); i++){
                if(maxPriority < quenes.get(i).get(0).getPriority()){
                    maxPriority = quenes.get(i).get(0).getPriority();
                    queneMax = i;
                }
            }
            r = quenes.get(queneMax).remove(0);
            r.setTimeBeginProc(time);
            if(quenes.get(queneMax).isEmpty()){
                quenes.remove(queneMax);
            }
            procStatus = false;
            stopStatus = false;
        }
        if(!procStatus && ((time - r.getTimeBeginProc()) >= r.getTimeService())){
            procStatus = true;
            r.setTimeEndProc(time);
            readyRequest.add(r);
            stopStatus = false;
        }
        if(stopStatus && procStatus){
            stopTime++;
        }
        stopStatus = true;
    }

    public void writeStatictic(double lamda) throws IOException {
        BufferedWriter file = new BufferedWriter(new FileWriter(new
                File("Stat_"+Double.toString(lamda)+".csv")));
        for(int i = 0; i < readyRequest.size(); i++){
            file.write(readyRequest.get(i).toString());
        }
        if(file != null){
            file.close();
        }
    }
    public int getAvarageTime(){
        int time = 0;
        for(int i = 0; i < readyRequest.size(); i++){
            time += readyRequest.get(i).getTimeBeginProc()-readyRequest.get(i).getTimeCome();
        }
        time /= readyRequest.size();
        time *= 10;
        time = Math.round(time);
        time /= 10;
        return time;
    }

    public int getStopTime(){
        return stopTime;
    }

    public int[] getTimePriority(){
        System.out.print(timePriority.length);
        for(int i = 0; i < timePriority.length; i++){
            timePriority[i] = 0;
            counter[i] = 0;
        }
        for(int i = 0; i < readyRequest.size(); i++){
            timePriority[readyRequest.get(i).getPriority()-1] += readyRequest.get(i).getTimeBeginProc()-
                    readyRequest.get(i).getTimeCome();
            counter[readyRequest.get(i).getPriority()-1]++;
        }
        for (int i = 0; i < timePriority.length; i++) {
            if(counter[i] > 0){
                timePriority[i] /= counter[i];
            }
        }
        return timePriority;
    }
}