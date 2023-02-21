import { formatDate } from '@angular/common';
import { HttpClient } from '@angular/common/http';
import { Component, OnInit } from '@angular/core';
import { IMqttMessage, MqttService } from 'ngx-mqtt';
import { PrimeNGConfig } from 'primeng/api';
import { Subscription } from 'rxjs';
import { Pm } from 'src/Pm';

@Component({
  selector: 'app-air-parameters',
  templateUrl: './air-parameters.component.html',
  styleUrls: ['./air-parameters.component.css']
})
export class AirParametersComponent implements OnInit{
  pm25:Number = -1;
  pm10:Number = -1;
  noise:Number = -1;
  temperature:Number = -1;
  humidity:Number = -1;
  lastPmUpdate: string = "unknown";
  pmSubscription: Subscription | undefined;
  allPmReadings: Pm[] = [];

  constructor(private _mqttService: MqttService,private http:HttpClient, private primengConfig: PrimeNGConfig) {
    this.primengConfig.ripple = true;
  }
  ngOnInit(): void {
    this.http.get<Pm>('/api/last/pm').subscribe(data => {
      this.lastPmUpdate = data.localDateTime;
      this.pm10 = data.pm10;
      this.pm25 = data.pm25;
      this.noise = data.noise;
      this.temperature = data.temperature;
      this.humidity = data.humidity;
    });
    this.pmSubscription = this._mqttService.observe('pm').subscribe((message: IMqttMessage) => {
      this.parseJsonAndUpdate(message.payload.toString(),'pm');
    });  
    this.http.get<Pm[]>('/api/findAllPmReadings').subscribe(data =>{
      this.allPmReadings = data;
      console.log(this.allPmReadings);
    });
  }
  parseJsonAndUpdate(message: string,topic: String) {
    let object = JSON.parse(message);
    console.log(object,message);
    if(topic == 'pm') {
      this.pm25 = object["pm25"];
      this.pm10 = object["pm10"];
      this.noise = object["noise"];
      this.temperature = object["temperature"];
      this.humidity = object["humidity"];
      this.lastPmUpdate = formatDate(new Date(),'HH:mm:ss dd MMM, yyyy','en');
      //update readings
    }
    let obj:any = {
      "humidity":this.humidity,
      "localDateTime":this.lastPmUpdate,
      "noise":this.noise,
      "pm10":this.pm10,
      "pm25":this.pm25,
      "temperature":this.temperature
    }
    this.allPmReadings = [obj].concat(this.allPmReadings);
  }
  getSeverity(measuring:Pm) {
    let count = 0;
    let text = "";
    if(measuring.humidity <= 30) {
      text = "warning"
      count++
    }
    if(measuring.humidity >= 60) {
      text = "warning"
      count++
    }
    if (measuring.temperature < 15) {
      text = "danger"
      count++
    }
    if (measuring.temperature > 24) {
      text = "danger"
      count++
    }
    if (measuring.noise >= 70 && measuring.noise < 80) {
      text = "warning"
      count++
    }
    if(measuring.noise >= 80) {
      text = "danger"
      count++
    }
    if (measuring.pm10 > 40 && measuring.pm10 <= 80) {
      text = "warning"
      count++
    }
    if (measuring.pm10 > 100) {
      text = "danger"
      count++
    }
    if (measuring.pm25 > 30 && measuring.pm25 <= 50) {
      text = "warning"
      count++
    }
    if (measuring.pm25 > 50) {
      text = "danger"
      count++
    }
    if(text == "") {
      return "success"
    }
    if(count>1) {
      return "danger"
    }
    else {
      return text;
    }
  }
  getText(measuring:Pm) {
    let text:String = ""
    if(measuring.humidity <= 30) {
      text+="low humidity - "
    }
    if(measuring.humidity >= 60) {
      text+= "high humidity - "
    }
    if (measuring.temperature < 15) {
      text+= "cold - "
    }
    if (measuring.temperature > 24) {
      text+= "hot - "
    }
    if (measuring.noise > 70) {
      text+= "loud - "
    }
    if (measuring.pm10 > 40 && measuring.pm10 <= 80) {
      text+= "medium pm10 - "
    }
    if (measuring.pm10 > 100) {
      text+= "high pm10 - "
    }
    if (measuring.pm25 > 30 && measuring.pm25 <= 50) {
      text+= "medium pm25 - "
    }
    if (measuring.pm25 > 50) {
      text+= "high pm25 - "
    }
    if(text == "") {
      return "normal"
    }
    return text.slice(0,-2);
  }
  getTemperatureSeverity() {
    if(this.temperature < 18 && this.temperature > 5 || this.temperature > 24 && this.temperature < 28) {
      return "warning"
    }
    else if(this.temperature < 5 || this.temperature > 28) {
      return "danger"
    }
    return "success"
  }
  getHumiditySeverity() {
    if(this.humidity <= 30 && this.humidity > 20 || this.humidity >= 60 && this.humidity <= 80) {
      return "warning"
    }
    if(this.humidity <= 20 || this.humidity >= 80) {
      return "danger"
    }
    return "success"
  }
  getPm10Severity() {
    if(this.pm10 >= 40 && this.pm10 <= 80) {
      return "warning"
    }
    if(this.pm10 >= 80) {
      return "danger"
    }
    return "success"
  }
  getPm25Severity() {
    if(this.pm25 >= 30 && this.pm25 <= 50) {
      return "warning"
    }
    if(this.pm25 >= 50) {
      return "danger"
    }
    return "success"
  }
  getNoiseSeverity() {
    if(this.noise >= 70 && this.noise < 80) {
      return "warning"
    }
    if(this.noise >= 80) {
      return "danger"
    }
    return "success"
  }
}
