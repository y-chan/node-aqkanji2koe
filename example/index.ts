import AqKanji2Koe from '@/index'

const aqkanji2koe = new AqKanji2Koe('AqKanji2Koe.dll', 'AqUsrDic.dll', 'aq_dic')

console.log(aqkanji2koe.AqKanji2KoeConvertUtf8('こんにちは'))
