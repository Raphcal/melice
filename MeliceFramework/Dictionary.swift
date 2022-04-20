//
//  Dictionary.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 19/04/2022.
//

import Foundation

public extension MELNonnullStringDictionary {
    subscript(key: String) -> String? {
        get {
            let value = UnsafeMutablePointer<MELNonnullString>.allocate(capacity: 1)
            let found = MELNonnullStringDictionaryGetIfPresent(self, key, value)
            let result: String? = found ? String(utf8String: value.pointee) : nil
            value.deallocate()
            return result
        }
        set(newValue) {
            if let newValue = newValue {
                MELNonnullStringDictionaryPut(&self, key, newValue)
            } else {
                let oldValue = MELNonnullStringDictionaryRemove(&self, key)
                free(UnsafeMutableRawPointer(mutating: oldValue))
            }
        }
    }
}
