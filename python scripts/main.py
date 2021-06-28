import os
import chunker
import postEdit
import preprocess

for f in os.listdir('../resource/editor/changes done/'):
    os.remove(f'../resource/editor/changes done/{f}')
chunker.main([])
postEdit.main([])
preprocess.main(['objecter'])
