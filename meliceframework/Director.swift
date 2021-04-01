//
//  Director.swift
//  meliceframework
//
//  Created by Raphaël Calabro on 31/03/2021.
//

import Melice

public extension MELDirector {
    mutating func startWith<T : MELSceneType>(_ scene: inout T) {
        MELDirectorStartWithScene(&self, &scene.super)
    }

    func draw() {
        MELDirectorDraw(self)
    }

    mutating func update(timeSinceLastUpdate: MELTimeInterval) {
        MELDirectorUpdateWithTimeSinceLastUpdate(&self, timeSinceLastUpdate)
    }
}
