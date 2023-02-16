import { Component, OnInit } from '@angular/core';
import { Message } from '../Message.model';
import { RestService } from '../rest.service';
import { Time } from "./Time.model";

@Component({
  selector: 'app-rightside',
  templateUrl: './rightside.component.html',
  styleUrls: ['./rightside.component.css']
})


export class RightsideComponent implements OnInit {

  hourTemp:string = '';
  minuteTemp:string = '';
  selectValue:any;
  postdata:any;
  positionTemp:any;
  
  registered_Times: Time[] = [];
  Time_positions: string[] = ["Open","Close"];

  constructor(private REST: RestService){ }

  ngOnInit(): void {
  }

  hourInput(event: any): void {
    this.hourTemp = event.target.value;
  }

  minuteInput(event: any): void{
    this.minuteTemp = event.target.value;
  }

  addNewTime(): void{
    if(this.timeValidation(this.hourTemp,this.minuteTemp)){
      if(!this.checkForTimes(this.hourTemp,this.minuteTemp,this.positionTemp)){
        this.registered_Times.push(new Time(this.hourTemp,this.minuteTemp,this.positionTemp));
        this.REST.refreshTime(new Message("windowBlind/refreshTime",this.registered_Times))
                .subscribe((data) => (this.postdata = data))          
      }else{
        //warning, time is already there
      }
    }else{
      //warning, invalid Time as Input
    }
  }

  deleteTime(): void{
    for(var i = this.registered_Times.length;i >= 0;i--){
      for(var tempindex of this.rIndexOf(this.selectValue)){
        if(i === tempindex)
          this.registered_Times.splice(tempindex,1);
      }
    }
    this.REST.refreshTime(new Message("windowBlind/refreshTime",this.registered_Times))
            .subscribe((data) => (this.postdata = data));
  }


  rIndexOf(times: Time[]):number[]{
    var indexes:number[] = [];

    for(var fortime1 of this.registered_Times){
      for(var fortime2 of times){
        if(fortime1.hour === fortime2.hour && fortime1.minute === fortime2.minute){
          indexes.push(this.registered_Times.indexOf(fortime1));
        }
      }
    }

    return indexes;
  }

  checkForTimes(hour:any, minute:any, position:any):boolean{//check if a time is already in the registered_times array
    for(var fortime of this.registered_Times){
      if((fortime.hour === hour && fortime.minute === minute) || fortime.position === position){
        return true;
      }
    }
    return false;
  }

  timeValidation(hour:string, minute:string):boolean{ //check if the time you give is valid
    if(hour != '' && minute != ''){
      if((parseInt(hour,10) <= 24 && parseInt(hour,10) >= 0) && (parseInt(minute,10) >= 0 && parseInt(minute,10) <= 59)){
        if(parseInt(hour,10) == 24 && parseInt(minute,10) != 0)
          return false;
        return true;
      }
      return false;
    }
    return false;
  }

  
}
