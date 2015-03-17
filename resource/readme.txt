Read http://qt.nokia.com/doc/4.6/resources.html for how to use resources.
Basicually you need to call

Q_INIT_RESOURCE(qfbconnectimages); // when you want to use the qfacebookconnect resources.
Q_CLEANUP_RESOURCE(qfbconnectimages); // when you no longer want to use the images resource and want to free some memory.

once you have the resources loaded you can access a particular resource like so:

cutAct = new QAction(QIcon(":/images/login2.png"), tr("Login to facebook"), this);