package com.example.smarthomespringmqtt.repository;

import com.example.smarthomespringmqtt.domain.WindowReading;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface WindowRepository extends JpaRepository<WindowReading,Long> {
}
