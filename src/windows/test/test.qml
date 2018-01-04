import QtQuick 2.0

Item {
    width: 640
        height: 480

        property alias button: button

//        Button {
//            anchors.centerIn: parent
//            id: button
//            text: qsTr("Press Me")
//        }
        states: [
            State {
                name: "State1"
            }
        ]
}
