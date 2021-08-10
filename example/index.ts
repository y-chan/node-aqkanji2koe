import AqKanji2Koe from '@/index'

let aqkanji2koe = new AqKanji2Koe('AqKanji2Koe.dll', 'AqUsrDic.dll', 'aq_dic')

aqkanji2koe.AqUsrDicImport('example/example.csv')

// reload
aqkanji2koe = new AqKanji2Koe('AqKanji2Koe.dll', 'AqUsrDic.dll', 'aq_dic')

console.log(aqkanji2koe.AqKanji2KoeConvertUtf8("GitHub"))
