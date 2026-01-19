//
//  View.swift
//  WindTunnelers
//
//  Created by Rahim Malik on 1/18/26.
//

import SwiftUI

struct ContentView: View {

    @StateObject private var sensors = SensorViewModel()
    @State private var serial: SerialController?

    private let cellWidth: CGFloat = 200

    var body: some View {
        VStack(spacing: 30) {
            VStack(spacing: 25) {
                FanIcon(velocity: sensors.velocity)
                Text("Welcome Wind Tunnelers")
                    .font(.system(size: 40, weight: .bold, design: .monospaced))
                Text("Patrick Kaczmarek, Rahim Malik, Kaylie Shelton, Madelyn Handly")
                    .font(.system(size: 20, weight: .medium, design: .monospaced))
            }
            .padding(.bottom, 15)

            DataCard {
                HStack(alignment: .center, spacing: 20) {
                    VStack(alignment: .leading, spacing: 5) {
                        Text("Cd•A")
                            .font(.system(size: 16, design: .monospaced))
                            .foregroundStyle(.blue.opacity(0.5))
                        // quick convert to cm^2
                        Text(sensors.cdA * 10000, format: .number.precision(.fractionLength(2)))
                            .font(.system(size: 36, weight: .semibold, design: .monospaced))
                            .foregroundStyle(.blue)
                        Text("cm²")
                            .font(.system(size: 16, design: .monospaced))
                            .foregroundStyle(.blue.opacity(0.5))
                    }
                    Text("=")
                        .font(.system(size: 30, weight: .bold, design: .monospaced))
                    VStack(spacing: 10) {
                        DataCell(label: "2F", value: sensors.data?.dragForceN ?? 0, unit: "N")
                        // division bar
                        Capsule()
                            .frame(height: 2)
                            .foregroundStyle(.primary.opacity(0.5))
                            .padding(.horizontal, 12)
                        HStack(spacing: 15) {
                            DataCell(label: "ρ", value: sensors.airDensity, unit: "kg/m³")
                            Text("•")
                                .font(.system(size: 22, weight: .bold, design: .monospaced))
                                .padding(.bottom, -10)
                                .padding(.leading, -15)
                                .padding(.trailing, 30)
                            DataCell(label: "V²", value: sensors.velocity * sensors.velocity, unit: "(m/s)²")
                        }
                    }
                }
                .padding(35)
            }
            .frame(maxWidth: .infinity)

            DataCard {
                LazyVGrid(columns: Array(repeating: GridItem(.flexible(minimum: cellWidth)), count: 3), spacing: 25) {
                    DataCell(label: "Temperature", value: sensors.data?.temperatureC ?? 0, unit: "°C")
                    DataCell(label: "Humidity", value: sensors.data?.humidity ?? 0, unit: "%")
                    DataCell(label: "Diff Pressure", value: sensors.data?.pitotDeltaPKPa ?? 0, unit: "kPa")
                    DataCell(label: "Velocity", value: sensors.velocity, unit: "m/s")
                    DataCell(label: "Air Density", value: sensors.airDensity, unit: "kg/m³")
                    DataCell(label: "Angle", value: sensors.data?.angleDeg ?? 0, unit: "deg")
                }
                .padding(25)
            }
            .frame(maxWidth: .infinity, maxHeight: .infinity)
        }
        .padding(25)
        .background(.ultraThinMaterial)
        .onAppear {
            serial = SerialController(sensors: sensors)
        }
    }
    
    struct FanIcon: View {
        
        let velocity: Double
        private var rotationSpeed: Double {
            max(0.2, min(velocity / 5.0, 4.0))
        }

        @State private var angle: Double = 0

        var body: some View {
            Image(systemName: "fanblades.fill")
                .font(.system(size: 60))
                .foregroundStyle(.blue)
                .rotationEffect(.degrees(angle))
                .onAppear {
                    angle = 360
                }
                .animation(
                    .linear(duration: 0.15 / rotationSpeed)
                        .repeatForever(autoreverses: false),
                    value: angle
                )
        }
    }
}

struct DataCell: View {
    
    let label: String
    let value: Double
    let unit: String
    let cellWidth: CGFloat = 200

    var body: some View {
        VStack(alignment: .leading, spacing: 5) {
            Text(label)
                .font(.system(size: 16, design: .monospaced))
                .opacity(0.5)
            HStack(alignment: .firstTextBaseline, spacing: 5) {
                Text(value, format: .number.precision(.fractionLength(3)))
                    .font(.system(size: 28, weight: .medium, design: .monospaced))
                Text(unit)
                    .font(.system(size: 16, design: .monospaced))
                    .opacity(0.5)
            }
        }
        .frame(width: cellWidth, alignment: .leading)
    }
}

struct DataCard<Content: View>: View {
    
    let content: Content

    init(@ViewBuilder content: () -> Content) {
        self.content = content()
    }

    var body: some View {
        RoundedRectangle(cornerRadius: 22, style: .continuous)
            .fill(.ultraThinMaterial)
            .overlay(
                RoundedRectangle(cornerRadius: 22, style: .continuous)
                    .strokeBorder(.white.opacity(0.1))
            )
            .overlay(content)
    }
}
