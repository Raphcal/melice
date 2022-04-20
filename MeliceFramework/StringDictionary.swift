//
//  StringDictionary.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 20/04/2022.
//

import Foundation

public extension MELStringDictionary {
    subscript (key: String) -> String? {
        get {
            if let value = MELStringDictionaryGet(self, key) {
                return String(utf8String: value)
            } else {
                return nil
            }
        }
        set(newValue) {
            if let oldValue = MELStringDictionaryPut(&self, key, newValue) {
                free(oldValue)
            }
        }
    }

    subscript (key: UnsafePointer<CChar>) -> String? {
        get {
            if let value = MELStringDictionaryGet(self, key) {
                return String(utf8String: value)
            } else {
                return nil
            }
        }
        set(newValue) {
            if let oldValue = MELStringDictionaryPut(&self, key, newValue) {
                free(oldValue)
            }
        }
    }

    func contains(key: String) -> Bool {
        return MELStringDictionaryContainsKey(self, key)
    }

    func contains(key: UnsafePointer<CChar>) -> Bool {
        return MELStringDictionaryContainsKey(self, key)
    }
}
