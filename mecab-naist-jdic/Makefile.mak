
all: char.bin matrix.bin sys.dic unk.dic

char.bin matrix.bin sys.dic unk.dic: naist-jdic.csv matrix.def left-id.def pos-id.def rewrite.def right-id.def char.def unk.def feature.def
	..\mecab\src\mecab-dict-index.exe -d . -o . -f UTF-8 -t UTF-8

left-id.def:
	cp _left-id.def left-id.def

rewrite.def:
	cp _rewrite.def rewrite.def

pos-id.def:
	cp _pos-id.def pos-id.def

right-id.def:
	cp _right-id.def right-id.def

clean:
	del char.bin matrix.bin sys.dic unk.dic
