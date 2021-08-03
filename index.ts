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

/**
 * AqKanji2KoeとAqUsrDicの関数をまとめてラップしたクラス
 */
class AqKanji2Koe implements IAqKanji2Koe {
  private readonly addon: IAqKanji2Koe

  /**
   * AqKanji2Koeクラスの初期化
   * AqKanji2Koeライブラリはライセンスの関係上、使用者自身で用意する必要があるため、
   * そのライブラリのパス(ファイル名まで含む)を指定する。
   * なお、aqDicPathのみフォルダのパスを指定すること。
   * aqDicPathは後から"AqKanji2KoeCreate"関数等でも設定可能であるが、
   * constructor内での指定を推奨。
   * aqDicPathを指定しない上、"AqKanji2KoeCreate"関数も実行しない場合、言語処理はできない。
   * 読み込みに失敗した場合、エラーを投げるので、try-catchでのエラーハンドリングを推奨。
   * @param {string} aqKanji2KoeLibraryPath - AqKanji2Koeライブラリのパス(絶対パス推奨)
   * @param {string} aqUsrDicLibraryPath - AqUsrDicライブラリのパス(絶対パス推奨)
   * @param {string} aqDicPath - aq_dic(辞書フォルダ)のパス(絶対パス推奨)
   */
  constructor(
    aqKanji2KoeLibraryPath: string,
    aqUsrDicLibraryPath: string,
    aqDicPath?: string
  ) {
    if (aqDicPath) {
      // eslint-disable-next-line @typescript-eslint/no-unsafe-assignment, @typescript-eslint/no-unsafe-call
      this.addon = new addon(
        aqKanji2KoeLibraryPath,
        aqUsrDicLibraryPath,
        aqDicPath
      )
    } else {
      // eslint-disable-next-line @typescript-eslint/no-unsafe-assignment, @typescript-eslint/no-unsafe-call
      this.addon = new addon(aqKanji2KoeLibraryPath, aqUsrDicLibraryPath)
    }
  }

  /**
   * 言語処理インスタンス生成(初期化)
   * 辞書の読み込みに失敗した場合、エラーを投げるので、try-catchでのエラーハンドリングを推奨。
   * @param {string} pathDir - 辞書のディレクトリを指定(最後に/が有っても無くても良い)
   * @return {null}
   */
  AqKanji2KoeCreate(pathDir: string): null {
    return this.addon.AqKanji2KoeCreate(pathDir)
  }

  /**
   * 言語処理インスタンス生成(初期化)
   * 呼び出し側で辞書データ(バイナリ)をメモリに読み込んでから指定
   * 初期化を高速化するためのメモリマップトファイルなどが使える。
   * ただし、この関数を使用した場合、AqUsrDicで始まる関数が全く使えなくなる上、
   * JavaScript/TypeScript上でメモリアドレス/ポインタを直接扱うことは、
   * 極めて稀で危険かつ難しいため、非推奨。
   * 辞書の読み込みに失敗した場合、エラーを投げるので、try-catchでのエラーハンドリングを推奨。
   * @param {string} pSysDic - システム辞書データ先頭アドレス
   * @param {string} pUserDic - ユーザ辞書データ先頭アドレス(使用しないときは0を指定)
   * @return {null}
   */
  AqKanji2KoeCreatePtr(pSysDic: number, pUserDic: number): null {
    return this.addon.AqKanji2KoeCreatePtr(pSysDic, pUserDic)
  }

  /**
   * 言語処理(漢字仮名交じりテキストを音声記号列に変換)
   * 全てのOSで使用可能。
   * @param {string} kanji - 音声記号列にしたい文字列
   * @return {string} - 音声記号列
   */
  AqKanji2KoeConvertUtf8(kanji: string): string {
    return this.addon.AqKanji2KoeConvertUtf8(kanji)
  }

  /**
   * 言語処理(漢字仮名交じりテキストを音声記号列に変換)
   * Windowsのみで使用可能。
   * それ以外ではundefinedを返すので注意
   * @param {string} kanji - 音声記号列にしたい文字列
   * @return {string | undefined} - 音声記号列もしくはundefined
   */
  AqKanji2KoeConvertSjis(kanji: string): string | undefined {
    if (this.addon.AqKanji2KoeConvertSjis) {
      return this.addon.AqKanji2KoeConvertSjis(kanji)
    }
    return undefined
  }

  /**
   * 言語処理(漢字仮名交じりテキストを音声記号列に変換)
   * Windowsのみで使用可能。
   * それ以外ではundefinedを返すので注意
   * @param {string} kanji - 音声記号列にしたい文字列
   * @return {string | undefined} - 音声記号列もしくはundefined
   */
  AqKanji2KoeConvertUtf16(kanji: string): string | undefined {
    if (this.addon.AqKanji2KoeConvertUtf16) {
      return this.addon.AqKanji2KoeConvertUtf16(kanji)
    }
    return undefined
  }

  /**
   * 開発ライセンスキーの設定用関数。言語処理をする前に一度呼び出す。
   * これにより評価版の制限がなくなる。
   * キーの解析を防ぐため不正なキーでもtrueを返す場合がある。このとき制限は解除されない。
   * なお、本来のAqKanji2Koeの仕様と違って結果が反転しているので注意(本来は正しければ0、正しくなければ1を返す)
   * @param {string} key - ライセンスキーを指定
   * @return {boolean} - ライセンスキーが正しければtrue、正しくなければfalseが返る
   */
  AqKanji2KoeSetDevKey(key: string): boolean {
    return this.addon.AqKanji2KoeSetDevKey(key)
  }

  /**
   * CSV辞書からconstructor等で指定したaqDicPath内にユーザ辞書(aq_user.dic)を生成(上書)
   * @param {string} pathDicCsv - CSV辞書ファイルのパスを指定(絶対パス推奨)
   * @return {boolean} - 読み込み正常終了時にtrue、それ以外でfalseが返る
   */
  AqUsrDicImport(pathDicCsv: string): boolean {
    return this.addon.AqUsrDicImport(pathDicCsv)
  }

  /**
   * ユーザ辞書(aq_user.dic)からCSV辞書を生成
   * @param {string} pathDicCsv - CSV辞書ファイルのパスを指定(絶対パス推奨)
   * @return {boolean} - 書き出し正常終了時にtrue、それ以外でfalseが返る
   */
  AqUsrDicExport(pathDicCsv: string): boolean {
    return this.addon.AqUsrDicExport(pathDicCsv)
  }

  /**
   * CSV辞書に見出し語を追加・修正するとき、ユーザ辞書の生成前に読み記号列の書式などをチェック
   * @param {string} surface - 見出し語文字列(UTF8)
   * @param {string} yomi - よみ記号列(アクセント付き発音記号列 UTF8)
   * @param {number} posCode - 品詞コード(AqKanji2Koeライブラリ付属マニュアル参照)
   * @return {string | null} - エラーがあればstringが、なければnullが返る
   */
  AqUsrDicCheck(surface: string, yomi: string, posCode: number): string | null {
    return this.addon.AqUsrDicCheck(surface, yomi, posCode)
  }
}

export default AqKanji2Koe
