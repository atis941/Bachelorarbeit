export class Time {
    public hour:string;
    public minute:string;
    public position:string;
    public hour_and_minute:string;

  constructor(hour:string, minute:string, position:string){
    this.hour = hour;
    this.minute = minute;
    this.position = position;
    this.hour_and_minute = this.toString()
  }

  toString():string {
    return this.hour + ":" + this.minute + "/" + this.position;
  }
}