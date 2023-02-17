import { Component, OnInit } from '@angular/core';
import { Injectable } from '@angular/core';
import { MqttClientService, IMqttMessage, MqttService } from 'ngx-mqtt';
import { Subscription, BehaviorSubject,switchMap } from 'rxjs';
import {HttpClient} from '@angular/common/http'
import { formatDate } from '@angular/common';
import {InputSwitchModule} from 'primeng/inputswitch';
import { PrimeNGConfig } from 'primeng/api';
import { MenuItem } from 'primeng/api'

@Injectable({
  providedIn: 'root'
})
@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit{
  messages: string[] = [];
  windowSubscription: Subscription | undefined;
  lightSubscription: Subscription | undefined;
  pmSubscription: Subscription | undefined;
  
  klimaOn: Boolean = false;
  windowOpen: Boolean = false;
  lastWindowUpdate: string = "unknown";
  auto: Boolean = false;

  movementDetected: Boolean = false;
  lightOn: Boolean = false;
  lastLightUpdate: string = "unknown";

  pm25:Number = -1;
  pm10:Number = -1;
  noise:Number = -1;
  temperature:Number = -1;
  humidity:Number = -1;
  pressure:Number = -1;
  lastPmUpdate: string = "unknown";

  constructor(private _mqttService: MqttService,private http:HttpClient, private primengConfig: PrimeNGConfig) {
    this.primengConfig.ripple = true;
  }

  ngOnInit(): void {
    //http.get('/api/last/window').subscribe(data => console.log(data));
    this.windowSubscription = this._mqttService.observe('window').subscribe((message: IMqttMessage) => {
      this.parseJsonAndUpdate(message.payload.toString(),'window');
    });
    this.lightSubscription = this._mqttService.observe('light').subscribe((message: IMqttMessage) => {
      this.parseJsonAndUpdate(message.payload.toString(),'light');
    });
    this.pmSubscription = this._mqttService.observe('pm').subscribe((message: IMqttMessage) => {
      this.parseJsonAndUpdate(message.payload.toString(),'pm');
    });  
  }
  updateKlimaState() {
    this._mqttService.publish('window',`
      {
        "klimaOn":${!this.klimaOn},
        "windowOpen":${this.windowOpen}
      }
    `).subscribe();
  }
  updateLightState() {
    this._mqttService.publish('light', `{
      "lightOn":${!this.lightOn},
      "movementDetected":${this.movementDetected}
    }`).subscribe()
  }
  parseJsonAndUpdate(message: string,topic: String) {
    let object = JSON.parse(message);
    console.log(object,message);
    if(topic == 'window') {
      this.klimaOn = JSON.parse(object["klimaOn"]);
      this.windowOpen = JSON.parse(object["windowOpen"]);
      this.lastWindowUpdate = formatDate(new Date(),'dd MMM, yyyy HH:mm:ss','en');
      //update button
    }
    if(topic == 'light') {
      this.lightOn = JSON.parse(object["lightOn"]);
      this.movementDetected = JSON.parse(object["movementDetected"]);
      this.lastLightUpdate = formatDate(new Date(),'dd MMM, yyyy HH:mm:ss','en');
      //update button
    }
    if(topic == 'pm') {
      this.pm25 = object["pm25"];
      this.pm10 = object["pm10"];
      this.noise = object["noise"];
      this.temperature = object["temperature"];
      this.humidity = object["humidity"];
      this.pressure = object["pressure"];
      this.lastPmUpdate = formatDate(new Date(),'dd MMM, yyyy HH:mm:ss','en');
      //update readings
    }
  }

}