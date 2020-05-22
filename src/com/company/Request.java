package com.company;

public class Request {

    private int priority;
    private int id;
    private int timeService;
    private int timeCome;
    private int timeBeginProc;
    private int timeEndProc;

    public Request(int priority, int id, int timeService, int timeCome){
        this.priority = priority;
        this.id = id;
        this.timeService = timeService;
        this.timeCome = timeCome;
    }

    public int getTimeEndProc() {
        return timeEndProc;
    }

    public void setTimeEndProc(int timeEndProc) {
        this.timeEndProc = timeEndProc;
    }

    public int getTimeBeginProc() {
        return timeBeginProc;
    }

    public void setTimeBeginProc(int timeBeginProc) {
        this.timeBeginProc = timeBeginProc;
    }

    public int getPriority() {
        return priority;
    }

    public void setPriority(int priority) {
        this.priority = priority;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getTimeService() {
        return timeService;
    }

    public void setTimeService(int timeService) {
        this.timeService = timeService;
    }

    public int getTimeCome() {
        return timeCome;
    }

    public void setTimeCome(int timeCome) {
        this.timeCome = timeCome;
    }

    public String toString(){
        return Integer.toString(id)+";"+Integer.toString(priority)+";"+Integer.toString(timeService)+";"
                +Integer.toString(timeCome)+";"+Integer.toString(timeBeginProc)+";"+Integer.toString(timeEndProc)+"\n";
    }
}