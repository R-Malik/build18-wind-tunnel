//
//  Serial.swift
//  WindTunnelers
//
//  Created by Rahim Malik on 1/18/26.
//

import Foundation
import ORSSerial

final class SerialController: NSObject, ORSSerialPortDelegate {

    private let sensors: SensorViewModel
    private var port: ORSSerialPort?
    private var buffer = ""

    init(sensors: SensorViewModel) {
        self.sensors = sensors
        super.init()

        let manager = ORSSerialPortManager.shared()
        manager.addObserver(self, forKeyPath: "availablePorts", options: [.new], context: nil)

        print("Serial controller initialized")
        printAvailablePorts()
        attemptToOpenTeensy()
    }

    deinit {
        ORSSerialPortManager.shared().removeObserver(self, forKeyPath: "availablePorts")
        port?.close()
    }

    private func attemptToOpenTeensy() {
        guard port == nil else { return }

        guard let teensyPort = ORSSerialPortManager.shared().availablePorts.first(where: {
            $0.path.contains("usbmodem")    // typically what it says in ports list
        }) else {
            print("No usbmodem port found yet")
            return
        }

        print("Attempting to open port: \(teensyPort.path)")

        teensyPort.baudRate = 115200    // same as firmware
        teensyPort.delegate = self
        teensyPort.open()

        self.port = teensyPort
    }

    // automatic recovery
    override func observeValue(forKeyPath keyPath: String?, of object: Any?, change: [NSKeyValueChangeKey : Any]?, context: UnsafeMutableRawPointer?) {
        if keyPath == "availablePorts" {
            print("Serial ports changed")
            printAvailablePorts()
            attemptToOpenTeensy()
        }
    }
    
    func serialPort(_ serialPort: ORSSerialPort, didReceive data: Data) {
        print("Received \(data.count) bytes")

        guard let chunk = String(data: data, encoding: .utf8) else {
            print("Failed to decode incoming data")
            return
        }

        print("Chunk:")
        print(chunk)
        
        buffer.append(chunk)
        // replacement required for CSV style parsing
        buffer = buffer.replacingOccurrences(of: "\r\n", with: "\n")
        buffer = buffer.replacingOccurrences(of: "\r", with: "\n")

        while let range = buffer.range(of: "\n") {
            let line = String(buffer[..<range.lowerBound])
            buffer.removeSubrange(..<range.upperBound)

            let trimmed = line.trimmingCharacters(in: .whitespaces)
            guard !trimmed.isEmpty else { continue }

            sensors.update(from: trimmed)
        }
    }

    func serialPort(_ serialPort: ORSSerialPort, didEncounterError error: Error) {
        print("Serial error: \(error.localizedDescription)")
    }

    func serialPortWasRemovedFromSystem(_ serialPort: ORSSerialPort) {
        print("Serial port removed from system: \(serialPort.path)")
        serialPort.close()
    }

    // MARK: - DEBUG
    private func printAvailablePorts() {
        print("Available serial ports:")
        for p in ORSSerialPortManager.shared().availablePorts {
            print("  \(p.path)")
        }
    }
    
    func serialPortWasOpened(_ serialPort: ORSSerialPort) {
        print("Serial port opened: \(serialPort.path)")
    }

    func serialPortWasClosed(_ serialPort: ORSSerialPort) {
        print("Serial port closed: \(serialPort.path)")
        if self.port === serialPort {
            self.port = nil
        }
    }
}
