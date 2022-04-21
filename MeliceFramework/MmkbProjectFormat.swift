//
//  MmkbProjectFormat.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 04/04/2022.
//

import Foundation

public struct MELMmkbProjectFormat {
    static let defaultAnimationNames = [
        "stand", "walk", "run", "skid", "jump", "fall", "shaky", "bounce", "duck", "raise", "appear", "disappear", "attack", "hurt", "die"
    ]

    public static func project(fileWrappers files: [String: FileWrapper]) throws -> MELProject {
        guard let infoPlist = files["Info.plist"]?.regularFileContents
        else {
            throw CocoaError(.fileReadCorruptFile)
        }
        let projectInfo = try PropertyListSerialization.propertyList(from: infoPlist, options: [], format: nil)

        guard let projectInfo = projectInfo as? [String: Any],
              let versionNumber = projectInfo["version"] as? Int,
              let palettes = projectInfo["palettes"] as? [String],
              let maps = projectInfo["maps"] as? [[String: String]],
              let sprites = projectInfo["sprites"] as? [String]
        else {
            throw CocoaError(.fileReadCorruptFile)
        }

        var format = MELMmkProjectFormat
        if versionNumber > Int(format.version) {
            throw CocoaError(.fileReadUnsupportedScheme)
        }

        format.version = UInt32(versionNumber)
        var project = MELProject()

        // Animation names
        let animationNames = projectInfo["animation-names"] as? [String] ?? defaultAnimationNames
        project.animationNames = MELStringListMakeWithInitialCapacity(animationNames.count)
        for animationName in animationNames {
            animationName.withCString { cString in
                MELStringListPush(&project.animationNames, MELStringCopy(cString))
            }
        }

        // Scripts
        if let scripts = projectInfo["scripts"] as? [String] {
            for index in 0 ..< scripts.count {
                if let file = files["script\(index).txt"],
                   let data = file.regularFileContents,
                   let content = String(data: data, encoding: .utf8) {
                    project.scripts[scripts[index]] = content
                }
            }
        }

        // Palettes
        var hasDefaultColorPalette = false
        for palette in palettes {
            if let data = files[palette]?.regularFileContents {
                let mutableData = NSMutableData(data: data)
                var inputStream = MELInputStreamInitWithBytes(mutableData.mutableBytes, mutableData.length)

                let palette = format.class.pointee.readPalette!(&format, &project, &inputStream)
                MELInputStreamDeinit(&inputStream)

                if let palette = palette {
                    MELPaletteRefListPush(&project.palettes, palette)
                    hasDefaultColorPalette = hasDefaultColorPalette || palette.name == "Default color palette"
                }
            }
        }
        if !hasDefaultColorPalette {
            let defaultColorPalette = MELPaletteRefAllocDefaultColorPalette()
            MELPaletteRefListPush(&project.palettes, defaultColorPalette)
        }

        // Maps and sprite instances
        for mapAndInstances in maps {
            if let mapFilename = mapAndInstances["map"],
               let mapData = files[mapFilename]?.regularFileContents,
               let instancesFilename = mapAndInstances["instances"],
               let instancesData = files[instancesFilename]?.regularFileContents {
                var mutableData = NSMutableData(data: mapData)
                var inputStream = MELInputStreamInitWithBytes(mutableData.mutableBytes, mutableData.length)
                var map = format.class.pointee.readMap!(&format, &project, &inputStream)
                MELInputStreamDeinit(&inputStream)

                let groundIndex = map.layers.firstIndex { layer in layer.isSolid || layer.nameAsString == "Piste" } ?? 0
                var ground = map.layers[groundIndex]

                mutableData = NSMutableData(data: instancesData)
                inputStream = MELInputStreamInitWithBytes(mutableData.mutableBytes, mutableData.length)
                let count = MELInputStreamReadInt(&inputStream)
                for _ in 0 ..< count {
                    let instance = format.class.pointee.readSpriteInstance!(&format, &project, &inputStream)
                    MELSpriteInstanceListPush(&ground.sprites, instance)
                }
                MELInputStreamDeinit(&inputStream)

                map.layers[groundIndex] = ground

                MELMutableMapListPush(&project.root.maps, map)
            }
        }

        // Sprites
        for sprite in sprites {
            if let data = files[sprite]?.regularFileContents {
                let mutableData = NSMutableData(data: data)
                var inputStream = MELInputStreamInitWithBytes(mutableData.mutableBytes, mutableData.length)

                let spriteDefinition = format.class.pointee.readSpriteDefinition!(&format, &project, &inputStream)
                MELInputStreamDeinit(&inputStream)

                if let motionName = spriteDefinition.motionName, strlen(motionName) > 0,
                   !project.scripts.contains(key: motionName) {
                    project.scripts[motionName] = ""
                }

                MELSpriteDefinitionListPush(&project.root.sprites, spriteDefinition)
            }
        }

        return project
    }

    public static func fileWrapper(project: MELProject) throws -> FileWrapper {
        var fileWrappers = [String: FileWrapper]()
        var info = [String: Any]()

        var format = MELMmkProjectFormat
        info["version"] = format.version

        info["animation-names"] = project.animationNames.map { animationName in
            animationName != nil ? String(cString: animationName!) : ""
        }
        info["next-map"] = project.root.maps.count

        var outputStream = MELOutputStreamInit()

        // Palettes
        var palettes = [String]()
        for index in 0 ..< project.palettes.count {
            let filename = "palette\(index).pal"
            outputStream.count = 0

            let palette = project.palettes[index]
            format.class.pointee.writePalette!(&format, project, &outputStream, palette)
            fileWrappers[filename] = .init(regularFileWithContents: Data(bytes: outputStream.buffer, count: Int(outputStream.count)))
            palettes.append(filename)
        }
        info["palettes"] = palettes

        // Maps and sprite instances
        var maps = [[String: String]]()
        for index in 0 ..< project.root.maps.count {
            var mapAndInstances = [String: String]()

            let mapFilename = "map\(index).map"
            let instancesFilename = "map\(index).ins"
            mapAndInstances["map"] = mapFilename
            mapAndInstances["instances"] = instancesFilename
            maps.append(mapAndInstances)

            outputStream.count = 0
            let map = project.root.maps[index]
            format.class.pointee.writeMap!(&format, project, &outputStream, map)
            fileWrappers[mapFilename] = .init(regularFileWithContents: Data(bytes: outputStream.buffer, count: Int(outputStream.count)))

            outputStream.count = 0
            let instances = map.layers.flatMap({ layer in layer.sprites })
            MELOutputStreamWriteInt(&outputStream, Int32(instances.count))
            for instance in instances {
                format.class.pointee.writeSpriteInstance!(&format, project, &outputStream, instance)
            }
            fileWrappers[instancesFilename] = .init(regularFileWithContents: Data(bytes: outputStream.buffer, count: Int(outputStream.count)))
        }
        info["maps"] = maps

        // Sprites
        var sprites = [String]()
        for index in 0 ..< project.root.sprites.count {
            let filename = "sprite\(index).spr"
            outputStream.count = 0

            let sprite = project.root.sprites[index]
            format.class.pointee.writeSpriteDefinition!(&format, project, &outputStream, sprite)
            fileWrappers[filename] = .init(regularFileWithContents: Data(bytes: outputStream.buffer, count: Int(outputStream.count)))
            sprites.append(filename)
        }
        info["sprites"] = sprites

        MELOutputStreamDeinit(&outputStream)

        // Scripts
        var scripts = [String]()
        for key in project.scripts.keys {
            if let key = String(utf8String: key),
               let scriptData = project.scripts[key]?.data(using: .utf8) {
                fileWrappers["script\(scripts.count).txt"] = .init(regularFileWithContents: scriptData)
                scripts.append(key)
            }
        }
        info["scripts"] = scripts

        fileWrappers["Info.plist"] = .init(regularFileWithContents: try PropertyListSerialization.data(fromPropertyList: info, format: .xml, options: 0))
        return .init(directoryWithFileWrappers: fileWrappers)
    }
}
