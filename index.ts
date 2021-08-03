// eslint-disable-next-line @typescript-eslint/no-unsafe-assignment, @typescript-eslint/no-unsafe-call
const addon = require('bindings')('aqkanji2koe')

export interface IAqKanji2Koe {
  AqKanji2KoeCreate(pathDir: string): null
  AqKanji2KoeCreatePtr(pSysDic: number, pUserDic: number): null
  AqKanji2KoeConvertUtf8(kanji: string): string
  AqKanji2KoeConvertSjis(kanji: string): string | undefined
  AqKanji2KoeConvertUtf16(kanji: string): string | undefined
  AqKanji2KoeSetDevKey(key: string): boolean
  AqUsrDicImport(pathDicCsv: string): boolean
  AqUsrDicExport(pathDicCsv: string): boolean
  AqUsrDicCheck(surface: string, yomi: string, posCode: number): string | null
}

class AqKanji2Koe implements IAqKanji2Koe {
  private readonly addon: IAqKanji2Koe

  constructor(aqKanji2KoeLibraryPath: string, aqUsrDicLibraryPath: string, aqDicPath?: string) {
    if (aqDicPath) {
      // eslint-disable-next-line @typescript-eslint/no-unsafe-assignment, @typescript-eslint/no-unsafe-call
      this.addon = new addon(aqKanji2KoeLibraryPath, aqUsrDicLibraryPath, aqDicPath)
    } else {
      // eslint-disable-next-line @typescript-eslint/no-unsafe-assignment, @typescript-eslint/no-unsafe-call
      this.addon = new addon(aqKanji2KoeLibraryPath, aqUsrDicLibraryPath)
    }
  }

  AqKanji2KoeCreate(pathDir: string): null {
    return this.addon.AqKanji2KoeCreate(pathDir)
  }

  AqKanji2KoeCreatePtr(pSysDic: number, pUserDic: number): null {
    return this.addon.AqKanji2KoeCreatePtr(pSysDic, pUserDic)
  }

  AqKanji2KoeConvertUtf8(kanji: string): string {
    return this.addon.AqKanji2KoeConvertUtf8(kanji)
  }

  AqKanji2KoeConvertSjis(kanji: string): string | undefined {
    if (this.addon.AqKanji2KoeConvertSjis) {
      return this.addon.AqKanji2KoeConvertSjis(kanji)
    }
    return undefined
  }

  AqKanji2KoeConvertUtf16(kanji: string): string | undefined {
    if (this.addon.AqKanji2KoeConvertUtf16) {
      return this.addon.AqKanji2KoeConvertUtf16(kanji)
    }
    return undefined
  }

  AqKanji2KoeSetDevKey(key: string): boolean {
    return this.addon.AqKanji2KoeSetDevKey(key)
  }

  AqUsrDicImport(pathDicCsv: string): boolean {
    return this.addon.AqUsrDicImport(pathDicCsv)
  }

  AqUsrDicExport(pathDicCsv: string): boolean {
    return this.addon.AqUsrDicExport(pathDicCsv)
  }

  AqUsrDicCheck(surface: string, yomi: string, posCode: number): string | null {
    return this.addon.AqUsrDicCheck(surface, yomi, posCode)
  }
}

export default AqKanji2Koe
