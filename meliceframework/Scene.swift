//
//  Scene.swift
//  meliceframework
//
//  Created by Raphaël Calabro on 31/03/2021.
//

import Melice

public extension MELScene {
    /// Load the assets and set up the scene when it is in unloaded state.
    mutating func load() {
        self.load(&self)
    }

    /// Reset the scene when it is in loaded state.
    mutating func reload() {
        self.reload(&self)
    }

    /// Free the assets of the scene after it has been hidden.
    mutating func unload() {
        self.unload(&self)
    }

    /// Prepare the scene right before being shown on screen.
    mutating func willAppear() {
        self.willAppear(&self)
    }

    /// Draw the scene content.
    mutating func draw() {
        self.draw(&self)
    }

    /// Update the scene.
    ///
    /// - parameter timeSinceLastUpdate: Time in seconds since the last call.
    mutating func update(timeSinceLastUpdate: MELTimeInterval) {
        self.update(&self, timeSinceLastUpdate)
    }
}
