printf -v sessionFileName 'stepAnswer_%(%Y-%m-%d_%Hh_%Mm_%Ss)T.session' -1
python3 ../../IT_Client/Client.py -initFile recordStepAnswer_procedure.py -sessionFile $sessionFileName
python3 PlotStepAnswer.py $sessionFileName
