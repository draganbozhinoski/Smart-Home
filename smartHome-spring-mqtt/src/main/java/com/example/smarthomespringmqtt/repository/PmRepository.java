package com.example.smarthomespringmqtt.repository;

import com.example.smarthomespringmqtt.domain.PmReading;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface PmRepository extends JpaRepository<PmReading,Long> {
}
