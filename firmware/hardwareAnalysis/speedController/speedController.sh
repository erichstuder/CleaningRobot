printf -v sessionFileName 'controllerSteps_%(%Y-%m-%d_%Hh_%Mm_%Ss)T.session' -1
lxterminal -e python3 PlotSpeedController.py $sessionFileName
python3 ../../IT_Client/Client.py -initFile speedController_procedure.py -sessionFile $sessionFileName
