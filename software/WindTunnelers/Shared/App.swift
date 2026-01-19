//
//  App.swift
//  WindTunnelers
//
//  Created by Rahim Malik on 1/18/26.
//

import SwiftUI
import AppKit

@main
struct WindTunnelersApp: App {
    var body: some Scene {
        WindowGroup {
            ContentView()
                .background(WindowAccessor())
        }
        .windowStyle(.hiddenTitleBar)
    }
}

// to make window transparent (add material later in view)
struct WindowAccessor: NSViewRepresentable {
    func makeNSView(context: Context) -> NSView {
        let view = NSView()

        DispatchQueue.main.async {
            if let window = view.window {
                window.titleVisibility = .hidden
                window.titlebarAppearsTransparent = true
                window.isOpaque = false
                window.backgroundColor = .clear
                window.hasShadow = true
            }
        }

        return view
    }

    func updateNSView(_ nsView: NSView, context: Context) {}
}
