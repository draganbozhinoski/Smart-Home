package com.example.smarthomespringmqtt.api;

import com.example.smarthomespringmqtt.domain.LightReading;
import com.example.smarthomespringmqtt.domain.PmReading;
import com.example.smarthomespringmqtt.domain.WindowReading;
import com.example.smarthomespringmqtt.service.UniversalService;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/api")
public class HomeController {
    private final UniversalService universalService;

    public HomeController(UniversalService universalService) {
        this.universalService = universalService;
    }

    @GetMapping("/last/window")
    public WindowReading getLastWindowState() {
        return universalService.findLastWindowReading();
    }
    @GetMapping("/last/light")
    public LightReading getLastLightReading() {
        return universalService.findLastLightReading();
    }
    @GetMapping("/last/pm")
    public PmReading getLastPmReading() {
        return universalService.findLastPmReading();
    }
}
