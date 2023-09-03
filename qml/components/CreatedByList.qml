import QtQuick 2.0
import Sailfish.Silica 1.0
import QtGraphicalEffects 1.0

ListView {
    visible: count > 0
    width: parent.width
    interactive: false
    height: contentHeight
    header: SectionHeader { text: qsTr("Created by") }
    delegate: Item {
        width: column.width
        height: Theme.itemSizeSmall + 2 * Theme.paddingSmall

        Rectangle {
            id: imageContainer

            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: Theme.horizontalPageMargin
            height: Theme.itemSizeSmall
            width: Theme.itemSizeSmall
            color: "transparent"

            Image {
                id: image

                property string logoUrl: model.profilePath === "" ? "" : (configurationDetailsModel.imagesSecureBaseUrl + configurationDetailsModel.profileSize + model.profilePath)

                anchors.fill: parent
                visible: configurationDetailsService.initialized
                source: logoUrl
                fillMode: Image.PreserveAspectCrop
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: mask
                }
            }

            Rectangle {
                id: mask

                anchors.fill: parent
                visible: false
                radius: width
            }

            Icon {
                id: body

                width: parent.width * 0.7
                height: width
                anchors.centerIn: parent
                source: "image://theme/icon-m-media-artists?" + Theme.primaryColor
                visible: image.status != Image.Ready
            }
        }

        Label {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: imageContainer.right
            anchors.right: parent.right
            width: parent.width - Theme.itemSizeLarge
            x: Theme.horizontalPageMargin + Theme.itemSizeSmall + Theme.paddingMedium
            text: model.name
            truncationMode: TruncationMode.Fade
            fontSizeMode: Text.HorizontalFit
            minimumPixelSize: Theme.fontSizeSmallBase
            leftPadding: Theme.paddingMedium
        }
    }
}