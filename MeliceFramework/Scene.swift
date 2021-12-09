//
//  Scene.swift
//  meliceframework
//
//  Created by Raphaël Calabro on 31/03/2021.
//

import Foundation

public protocol MELSceneType {
    var `super`: MELScene { get set }
}

public extension MELSceneType {
    /// Load the assets and set up the scene when it is in unloaded state.
    mutating func load() {
        self.super.load(&self.super)
    }

    /// Reset the scene when it is in loaded state.
    mutating func reload() {
        self.super.reload(&self.super)
    }

    /// Free the assets of the scene after it has been hidden.
    mutating func unload() {
        self.super.unload(&self.super)
    }

    /// Prepare the scene right before being shown on screen.
    mutating func willAppear() {
        self.super.willAppear(&self.super)
    }

    /// Draw the scene content.
    mutating func draw() {
        self.super.draw(&self.super)
    }

    /// Update the scene.
    ///
    /// - parameter timeSinceLastUpdate: Time in seconds since the last call.
    mutating func update(timeSinceLastUpdate: MELTimeInterval) {
        self.super.update(&self.super, timeSinceLastUpdate)
    }
}
