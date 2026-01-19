//
//  Model.swift
//  WindTunnelers
//
//  Created by Rahim Malik on 1/18/26.
//

import Foundation
import Combine

struct SensorData {
    let timestampMs: Int        // ms
    let temperatureC: Double    // Celcius
    let pitotDeltaPKPa: Double  // kPa
    let humidity: Double        // %
    let angleDeg: Double        // Deg
    let dragForceN: Double      // Newtons
}

final class SensorViewModel: ObservableObject {

    @Published var data: SensorData?
    @Published var airDensity: Double = 0.0
    @Published var velocity: Double = 0.0
    @Published var cdA: Double = 0.0

    // run on each new line in serial monitor
    func update(from csvLine: String) {
        let parts = csvLine.split(separator: ",").map(String.init)
        guard parts.count == 6 else {
            print("Invalid CSV field count:", csvLine)
            return
        }

        guard
            let timestamp = Int(parts[0]),
            let tempC = Double(parts[1]),
            let deltaPKPa = Double(parts[2]),
            let humidity = Double(parts[3]),
            let angle = Double(parts[4]),
            let forceN = Double(parts[5])
        else {
            print("CSV parse failed:", csvLine)
            return
        }

        let rho = computeAirDensity(temperatureC: tempC, relativeHumidity: humidity)
        let v = computeVelocity(deltaPKPa: deltaPKPa, density: rho)

        let cdAValue: Double
        if v > 0 {
            cdAValue = (2.0 * forceN) / (rho * v * v)
        } else {
            cdAValue = 0.0
        }

        // absolute value everything, prevent NaN
        DispatchQueue.main.async {
            self.data = SensorData(
                timestampMs: timestamp,
                temperatureC: abs(tempC),
                pitotDeltaPKPa: abs(deltaPKPa),
                humidity: abs(humidity),
                angleDeg: abs(angle),
                dragForceN: abs(forceN)
            )

            self.airDensity = abs(rho)
            self.velocity = abs(v)
            self.cdA = abs(cdAValue)
        }
    }


    // use standard atmospheric pressure for static
    private func computeAirDensity(temperatureC: Double, relativeHumidity: Double, staticPressurePa: Double = 101_325.0) -> Double {
        
        let T = temperatureC + 273.15   // Kelvin

        // Magnus-Tetens saturation vapor pressure (Pa) formula
        let es = 610.94 * exp((17.625 * temperatureC) / (temperatureC + 243.04))
        let pv = (relativeHumidity / 100.0) * es
        let pd = staticPressurePa - pv

        let Rd = 287.05   // J/(kg•K)
        let Rv = 461.5

        let density = abs((pd / (Rd * T)) + (pv / (Rv * T)))
        if density.isNaN || density.isInfinite {
            print("Invalid air density:", density)
            return 0.0
        }

        return density
    }

    private func computeVelocity(deltaPKPa: Double, density: Double) -> Double {
        if density <= 0 {
            print("Velocity calc skipped, invalid density:", density)
            return 0.0
        }

        let deltaPPa = deltaPKPa * 1000.0
        let effectiveDeltaP = abs(deltaPPa)

        // small pressures are just 0
        if effectiveDeltaP < 1.0 {
            return 0.0
        }

        let argument = 2.0 * effectiveDeltaP / density
        if argument <= 0 || argument.isNaN || argument.isInfinite {
            print("Invalid sqrt argument:", argument)
            return 0.0
        }

        return sqrt(argument)   // important that it's not NaN now
    }
}
