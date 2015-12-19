import jieba
import jieba.analyse

def split(toCut):
    jieba.initialize()
    
    toCut = unicode(toCut.decode("gbk"))
    retList = list(jieba.analyse.extract_tags(toCut, topK = 255, withWeight=1))
    retList = [i for i in retList if i[0] != ' ']
    
    for i in range(0, len(retList)):
        retList[i] = [retList[i][0].encode('GBK'), retList[i][1]]
        
    return retList


def utf2gbk(s):
    s = s.decode('utf-8')
    s = s.encode('gbk')
    return s


def wordDctChangeMode(filename):
    f_obj = open(filename,'r')
    output_obj = open('lexicon_gbk.lst','w')
    for line in f_obj:
        res = line.split('\t')
        try:
            new_line = utf2gbk(res[0]) + '\t' + res[1]
            output_obj.write(new_line)
        except Exception,ex:
            print line
    output_obj.close()

if __name__ == '__main__':
    '''wordDctChangeMode('lexicon.lst')
'''
    pass

