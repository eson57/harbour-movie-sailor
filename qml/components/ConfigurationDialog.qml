import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: dialog

    property string entityId: "null"
    property string entityLabel
    property var service
    property var requestInfo
    property var model
    property string title

    allowedOrientations: Orientation.All
    clip: true
    onStatusChanged: {
        pageContainer.anchors.bottomMargin = Theme.itemSizeMedium
    }
    canAccept: entityId !== "null"

    Item {
        visible: dialog.requestInfo.state === 1
        anchors.fill: parent

        BusyIndicator {
            id: busyIndicator
            running: true
            size: BusyIndicatorSize.Large
            anchors.centerIn: parent
        }

        ProgressBar {
            id: progressBar

            anchors {
                top: busyIndicator.bottom
                topMargin: Theme.paddingMedium
                horizontalCenter: parent.horizontalCenter
            }
            width: parent.width
            maximumValue: 100
            value: dialog.requestInfo.progress
        }
    }

    Label {
        visible: dialog.requestInfo.state === 3
        text: dialog.requestInfo.error === "" ? "Oops. Unknown error" : dialog.requestInfo.error
        color: Theme.errorColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.Wrap
        x: Theme.horizontalPageMargin
        width: parent.width - 2 * Theme.horizontalPageMargin
    }

    SilicaListView {
        visible: dialog.service.initialized
        model: dialog.model
        anchors.fill: parent
        currentIndex: -1 // otherwise currentItem will steal focus
        header:  Column {
            width: parent.width
            PageHeader {
                title: dialog.title
            }
        }

        section {
            property: 'section'

            delegate: SectionHeader {
                text: section
                height: Theme.itemSizeExtraSmall
            }
        }

        delegate: BackgroundItem {
            highlighted: model.id === dialog.entityId
            onClicked: {
                dialog.entityId = model.id
                dialog.entityLabel = model.label
            }

            Label {
                width: parent.width - 2 * Theme.horizontalPageMargin
                x: Theme.horizontalPageMargin
                anchors.verticalCenter: parent.verticalCenter
                font.bold: model.isPrimary
                color: highlighted ? Theme.highlightColor : Theme.primaryColor
                text: qsTr(model.label)
                truncationMode: TruncationMode.Fade
                wrapMode: "WordWrap"
            }
        }

        VerticalScrollDecorator {}

    }
}
