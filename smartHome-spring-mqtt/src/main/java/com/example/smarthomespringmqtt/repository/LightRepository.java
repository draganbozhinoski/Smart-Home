package com.example.smarthomespringmqtt.repository;

import com.example.smarthomespringmqtt.domain.LightReading;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface LightRepository extends JpaRepository<LightReading,Long> {
}
