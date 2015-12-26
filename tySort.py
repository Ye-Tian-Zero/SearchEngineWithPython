import sys

class TableObj:
    def __init__(self,doc_id,hit_times):
        self.doc_id_ = doc_id;
        self.hit_times_ = hit_times

def printResult(word_id,temp_dct,cout):
    temp_lst = sorted(value_dct.items(),key = lambda value_dct:value_dct[0])
    for obj in temp_lst:
        cout.write("%s\t%s\t%s\n"%(word_id,obj[0],obj[1]))



if __name__ == "__main__":
    cin = open("backwardIndex.lst")
    cout = open("backwardIndex_sorted.lst",'w')
    prev_result = []
    curr_result = []
    lines = cin.readlines()
    value_dct = {}
    for line in lines:
        curr_result = list(line.split('\t'))

        for i in range(len(curr_result)):
            curr_result[i] = int(curr_result[i])

        if(len(value_dct) == 0 and len(prev_result)== 3):
            value_dct[prev_result[1]] = prev_result[2]
        if(len(prev_result) !=  0):
            if(curr_result[0] == prev_result[0]):
                value_dct[curr_result[1]] = curr_result[2]
            else:
                printResult(prev_result[0],value_dct,cout)
                value_dct = {}
            
        prev_result = curr_result

    printResult(curr_result[0],value_dct,cout)
    cout.close()
    cin.close()
