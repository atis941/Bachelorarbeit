export class Message {
    topic : string;
    payload: any;

    constructor(topic:string, payload:any){
        this.topic = topic;
        this.payload = payload; 
    }
}