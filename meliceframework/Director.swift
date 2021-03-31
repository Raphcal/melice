//
//  Director.swift
//  meliceframework
//
//  Created by Raphaël Calabro on 31/03/2021.
//

import Foundation

extension MELDirector {
    mutating func start<T: MELSceneType>(with scene: inout T) {
        MELDirectorStartWithScene(&self, &scene.super)
    }

    func draw() {
        MELDirectorDraw(self)
    }

    mutating func update(timeSinceLastUpdate: MELTimeInterval) {
        MELDirectorUpdateWithTimeSinceLastUpdate(&self, timeSinceLastUpdate)
    }
}
