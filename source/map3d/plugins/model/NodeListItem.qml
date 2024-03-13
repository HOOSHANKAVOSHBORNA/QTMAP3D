import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import Qt.labs.qmlmodels

import "Components"
import "style"

Item {
    id: rootItem

    readonly property int widthStyle: 950
    readonly property int heightStyle: 68
    readonly property color disableColor: Qt.rgba(Style.foregroundColor.r,
                                                  Style.foregroundColor.g,
                                                  Style.foregroundColor.b, 0.5)
    readonly property color fg20: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.2)
    readonly property color fg30: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.3)
    readonly property color fg50: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.5)
    readonly property color fg75: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.75)
    readonly property color bg20: Qt.rgba(Style.backgroundColor.r,
                                          Style.backgroundColor.g,
                                          Style.backgroundColor.b, 0.2)
    readonly property color bg75: Qt.rgba(Style.backgroundColor.r,
                                          Style.backgroundColor.g,
                                          Style.backgroundColor.b, 0.75)

    function isNumeric(s) {
        return !isNaN(s - parseFloat(s))
    }

    //    DebugInAnchor {
    //        text: 'I am added bro'
    //    }

    // TODO: remove this
    anchors.fill: parent

    property var tableModel
    property var filterManager

    //property var attackModel: undefined
    Text {
        id: txtObjectList
        text: "Object list"
        anchors.top: parent.top
        anchors.topMargin: 20 / Style.monitorRatio
        anchors.left: parent.left
        color: Style.foregroundColor
        font.family: Style.fontFamily
        font.pixelSize: Style.titleFontSize
        font.bold: true
    }

    //-------------------------------------------CategoryTag-------------------------
    Rectangle {
        id: rectCategoryTag
        color: Style.backgroundColor
        anchors.top: txtObjectList.bottom
        anchors.topMargin: 20
        //width: parent.width
        height: 26
        anchors.left: parent.left

        RowLayout {
            spacing: 5
            Repeater {
                id: repeaterNodeTypeFilter
                property int currentIndex: 0
                model: tableModel ? tableModel.categoryTagModel : undefined /*[1, 2, 3]*/
                delegate: Rectangle {
                    id: rectDelegateFilterData
                    color: "transparent"
                    border.color: index === repeaterNodeTypeFilter.currentIndex ? Style.hoverColor : Style.foregroundColor
                    width: txtNodeTypeFilter.implicitWidth
                    height: rectCategoryTag.height
                    radius: 15
                    Text {
                        id: txtNodeTypeFilter
                        //anchors.fill: parent
                        anchors.centerIn: parent
                        leftPadding: 15
                        rightPadding: 15
                        bottomPadding: 3
                        topPadding: 3
                        text: model.display
                        color: index === repeaterNodeTypeFilter.currentIndex ? Style.hoverColor : Style.foregroundColor
                        font.family: Style.fontFamily
                        font.pixelSize: Style.regularFontSize
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            repeaterNodeTypeFilter.currentIndex = index
                            tableModel.setFilterCategoryTag(
                                        txtNodeTypeFilter.text)
                        }
                    }
                }
            }
        }
    }

    //shadow rectangleMainSearch
    Rectangle {
        id: dropShadowRect
        //width: rectMainSearch.width
        //height: rectMainSearch.height
        color: "black"
        radius: rectMainSearch.radius
        opacity: 0.06
        z: -1
        anchors.left: rectMainSearch.left
        anchors.leftMargin: -5
        anchors.bottom: rectMainSearch.bottom
        anchors.bottomMargin: -5
        anchors.right: rectMainSearch.right
        anchors.rightMargin: -5
        anchors.top: rectMainSearch.top
        anchors.topMargin: -5
    }
    Rectangle {
        id: rectMainSearch
        color: Style.backgroundColor
        //border.color: "black"
        anchors.top: rectCategoryTag.bottom
        anchors.topMargin: 20
        //width: parent.width
        height: searchRect.height + filterRect.height + 20 + 20 //parent.heightStyle
        anchors.left: parent.left
        //anchors.leftMargin: 20
        anchors.right: parent.right
        //anchors.rightMargin: 20
        radius: 15

        // Rectangle {
        //     id: dropShadowRect
        //     property real offset: Math.min(parent.width * 0.03,
        //                                    parent.height * 0.03)
        //     color: "black"
        //     width: parent.width
        //     height: parent.height
        //     z: -1
        //     opacity: 0.06
        //     radius: rectMainSearch.radius + 2
        //     anchors.left: parent.left
        //     anchors.leftMargin: -offset
        //     anchors.top: parent.top
        //     anchors.topMargin: offset
        // }
        Rectangle {
            id: searchRect
            anchors.left: parent.left
            anchors.leftMargin: 10 / Style.monitorRatio
            height: 28 / Style.monitorRatio
            width: 300 / Style.monitorRatio
            anchors.top: parent.top //rectCategoryTag.bottom
            anchors.topMargin: 20 / Style.monitorRatio
            radius: 15
            color: rootItem.fg20
            IconImage {
                id: searchIcon
                anchors.left: parent.left
                anchors.leftMargin: 10 / Style.monitorRatio
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/resources/search.png" //"qrc:/Resources/search-icon.jpg"
                width: 24 / Style.monitorRatio
                height: 24 / Style.monitorRatio
                color: rootItem.fg75
            }
            TextField {
                id: txtSearch
                anchors.left: searchIcon.right
                anchors.right: parent.right
                //anchors.leftMargin: searchIcon.width
                placeholderText: "...Search"
                color: Style.foregroundColor
                font.family: Style.fontFamily
                font.pixelSize: Style.smallFontSize
                anchors.verticalCenter: parent.verticalCenter
                selectedTextColor: Style.backgroundColor
                selectionColor: Style.foregroundColor
                placeholderTextColor: rootItem.fg50
                //wrapMode: Text.WrapAnywhere
                background: Rectangle {
                    color: "transparent"
                    radius: 15
                }
                onTextChanged: {
                    tableModel.setFilterSearch(txtSearch.text)
                }
            }
        }

        //-------------------------------------------Filter-------------------------
        // BOOKMARK: mainfilter
        Rectangle {
            id: filterRect
            anchors.top: searchRect.bottom
            color: Style.backgroundColor //'royalblue'
            anchors.left: parent.left
            anchors.right: parent.right
            height: 135 / Style.monitorRatio /*69 / Style.monitorRatio*/

            //z: -1
            Filter {
                id: dropDown
                property int innerWidth
                anchors.top: filterRect.top
                anchors.left: parent.left
                filterManager: rootItem.filterManager
            }
        }
    }
    //------------------------------------------TabBar------------------------------
    Rectangle {
        id: categoryRect
        color: Style.backgroundColor
        width: rootItem.width
        height: 50
        anchors.top: rectMainSearch.bottom
        anchors.topMargin: 30
        anchors.left: rootItem.left
        anchors.right: rootItem.right

        property int currentIndex: 0

        TabBar {
            id: tabBar
            width: parent.width
            anchors.left: parent.left
            anchors.right: parent.right
            currentIndex: categoryRect.currentIndex
            Component.onCompleted: {
                console.log(currentIndex)
                //tableModel.filterStringColumn(repeater.itemAt(0).text)
                //console.log(tabMain.modelData)
            }

            Repeater {
                id: repeater
                model: tableModel ? tableModel.tabbarModel : undefined
                onModelChanged: {
                    tabBar.currentIndex = categoryRect.currentIndex
                }

                delegate: TabButton {
                    id: tabMain

                    Component.onCompleted: {

                        //categoryRect.currentIndex = categoryRect.currentIndex
                        //tabBar.currentIndex = categoryRect.currentIndex
                        //tableModel.filterStringColumn(repeater.itemAt(0).text)
                        //console.log(repeater.itemAt(0).text)
                    }

                    text: model.display

                    onClicked: {
                        tableModel.setFilterColumn(txtTabbar.text)
                        categoryRect.currentIndex = model.index
                    }
                    background: Rectangle {
                        id: tabBarBack
                        //implicitWidth: 20
                        //implicitHeight: 20
                        color: Style.backgroundColor
                        //radius: tabBarBacks.implicitWidth
                        Rectangle {
                            id: rectBackTabbar
                            width: parent.width
                            height: 2
                            color: categoryRect.currentIndex
                                   === model.index ? Style.foregroundColor : Style.disableColor
                            anchors.bottom: parent.bottom
                        }
                    }
                    contentItem: Text {
                        id: txtTabbar
                        //anchors.fill: parent
                        //anchors.centerIn: parent
                        anchors.bottom: tabBarBack.bottom
                        anchors.bottomMargin: 5
                        text: tabMain.text
                        font.family: Style.fontFamily
                        font.pixelSize: Style.regularFontSize
                        color: categoryRect.currentIndex
                               === model.index ? Style.foregroundColor : Style.disableColor
                    }
                }
            }
        }
    }

    Rectangle {
        width: parent.width
        height: 2
        color: Style.foregroundColor
        anchors.bottom: horizontalHeaderView.bottom
    }

    //---------------------------------------------HeaderData-----------------------------
    HorizontalHeaderView {
        id: horizontalHeaderView
        anchors.left: rootItem.left
        anchors.top: categoryRect.bottom
        syncView: tableview
        clip: true
        delegate: Rectangle {
            id: rectHorizontalHeaderView

            implicitHeight: 30
            implicitWidth: 50 //parent.width
            color: "transparent" //"#DEE3E6"

            Text {
                text: model.display
                color: Style.foregroundColor
                font.family: Style.fontFamily
                font.pixelSize: Style.regularFontSize
                //anchors.centerIn: parent
                anchors.left: model.column === 2 ? parent.left : undefined
                anchors.centerIn: model.column === 2 ? undefined : parent
                anchors.verticalCenter: parent.verticalCenter
                visible: model.column !== 0 && model.column !== 1
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    tableModel.sortTable(
                                model.index) // model : data index in c++
                }
            }
        }
    }

    RowLayout {
        id: tableContentSplitter

        anchors.top: horizontalHeaderView.bottom
        anchors.left: rootItem.left
        anchors.right: rootItem.right
        anchors.bottom: rootItem.bottom

        spacing: 0

        //----------------------------------------TableView-------------------------
        ScrollView {
            id: scrollViewTable

            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical: ScrollBar {
                width: 0
            }

            TableView {
                id: tableview

                anchors.fill: parent //tableviewRect
                selectionBehavior: TableView.SelectRows //tableview.SelectRows
                property int selectedRow: -1
                property int columnZero: 5 //tableview.width / tableModel.columnCount() / 7
                property int columnIcons: 35 //tableview.width / tableModel.columnCount() / 2
                property color attackRowColor: rootItem.fg20
                property int checkAttackIconRow: -1
                property int checkAttackIconColumn: -1
                property bool isAttackecd: false

                //columnSpacing: 1
                rowSpacing: 1
                clip: true

                model: tableModel

                //selectionModel: tableModel.selectModel()
                //            selectionModel: ItemSelectionModel {
                //                id: selectionID
                //                model: tableview.selectionModel
                //                //model: tableModel
                //            }
                readonly property int tableCellHeight: 30

                delegate: Rectangle {
                    id: delegateContainer

                    implicitHeight: tableview.tableCellHeight
                    color: 'transparent'

                    Rectangle {
                        width: parent.width
                        height: 1
                        color: Style.foregroundColor
                        opacity: 0.2
                        anchors.bottom: parent.bottom
                    }

                    Loader {
                        anchors.fill: parent

                        sourceComponent: {
                            if (model.background !== "notType") {
                                return backgroundDelegate
                            } else if (model.display !== "notType") {
                                return displayDelegate
                            } else if (model.decorate !== "notType") {
                                return decorateDelegate
                            } else {
                                return errorDelegate
                            }
                        }

                        Component {
                            id: backgroundDelegate

                            Rectangle {
                                anchors.fill: parent
                                color: model.background
                                radius: 10
                            }
                        }

                        Component {
                            id: decorateDelegate

                            Rectangle {
                                anchors.fill: parent
                                color: 'transparent'

                                IconImage {
                                    anchors.centerIn: parent
                                    source: {
                                        if (model.decorate.substring(
                                                    0, 5) === "qrc:/") {
                                            return model.decorate.substring(5)
                                        } else {
                                            return "qrc:/Resources/close.png"
                                        }
                                    }

                                    color: (column === tableview.checkAttackIconColumn
                                            && row === tableview.checkAttackIconRow) ? "#01AED6" : "#003569"
                                    width: 30
                                    height: 30
                                }
                            }
                        }

                        Component {
                            id: displayDelegate

                            Rectangle {
                                anchors.fill: parent
                                color: 'transparent'

                                Text {
                                    text: model.display

                                    elide: Text.ElideRight
                                    font.pixelSize: Style.regularFontSize
                                    font.family: Style.fontFamily
                                    color: Style.foregroundColor
                                    anchors.left: model.column === 2 ? parent.left : undefined
                                    anchors.centerIn: model.column === 2 ? undefined : parent
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                        }

                        Component {
                            id: errorDelegate

                            Rectangle {
                                anchors.fill: parent
                                color: 'red'

                                Text {
                                    anchors.centerIn: parent
                                    text: 'Error'
                                }
                            }
                        }
                    }
                }

                //                Rectangle {
                //                id: tableDelegate

                //                //implicitWidth: 120 //rootItem.width / tableModel.columnCount() //120 //rootItem.height / 3
                //                implicitHeight: 32
                //                radius: model.column !== 0 ? 10 : 0
                //                color: model.background === "notType" ? "#DEE3E6" : model.background

                //                clip: true

                //                //anchors.right: column === tableModel.columnCount()-1 ? parent.right : undefined
                //                required property bool selected
                //                property int idxRow: model.row
                //                //property int rowCount: tableModel.getColumnCount()
                //                property int columnCnt: tableModel.columnCount()
                //                IconImage {
                //                    id: icons

                //                    anchors.centerIn: parent
                //                    visible: model.decorate !== "notType"
                //                    source: {
                //                        if (model.decorate === "notType") {
                //                            return "qrc:/Resources/hand.png"
                //                        } else {
                //                            if (model.decorate.substring(0, 5) === "qrc:/") {
                //                                return model.decorate.substring(5)
                //                            } else {
                //                                return "qrc:/Resources/close.png"
                //                            }
                //                        }
                //                    }

                //                    color: (column === tableview.checkAttackIconColumn
                //                            && row === tableview.checkAttackIconRow) ? "#01AED6" : "#003569"
                //                    width: 30
                //                    height: 30
                //                }

                //                Text {
                //                    visible: model.display !== "notType"
                //                    text: model.display === "notType" ? "ERROR" : model.display

                //                    elide: Text.ElideRight
                //                    //                    width: parent.width - 3
                //                    font.pixelSize: 17 / Style.monitorRatio
                //                    font.family: Style.fontFamily
                //                    color: Style.foregroundColor
                //                    //anchors.fill: parent
                //                    //anchors.centerIn: parent
                //                    anchors.left: model.column === 2 ? parent.left : undefined
                //                    anchors.centerIn: model.column === 2 ? undefined : parent
                //                    anchors.verticalCenter: parent.verticalCenter

                //                    //                    visible: model.column !== 0 && model.column
                //                    //                             !== 1 && model.column !== tableModel.columnCount()
                //                    //                             - 3 && model.column !== tableModel.columnCount()
                //                    //                             - 2 && model.column !== tableModel.columnCount() - 1
                //                }
                //                Rectangle {
                //                    width: parent.width
                //                    height: 1
                //                    color: Style.foregroundColor
                //                    opacity: 0.2
                //                    anchors.bottom: parent.bottom
                //                }

                //                MouseArea {
                //                    anchors.fill: parent
                //                    onClicked: {

                //                        //selectionID.select(tableview.selectionModel.model.index(row,0), ItemSelectionModel.ClearAndSelect | ItemSelectionModel.Rows)
                //                        //tableview.selectionModel.select(tableview.selectionModel.index(0,2), ItemSelectionModel.Toggle | ItemSelectionModel.Rows)
                //                        //tableview.selectedRow = model.row
                //                        //tableModel.selectionRow(row, 18)
                //                        // console.log("Row index: ", model.row, "column", columnCnt)
                //                        //console.log("column count : ", rowCount)
                //                        //console.log("column filter count: ", tableModel.columnCount())
                //                        //model.column === 11

                //                        /*if (decorate === "qrc:/Resources/more-icon.jpg") {
                //                            menuTable.popup()
                //                        } else if (decorate === "qrc:/Resources/battle-icon.jpg"
                //                                   && model.row === 0
                //                                   && tableview.checkAttackIconRow !== -1) {
                //                            console.log("chnage model")
                //                            tableModel.setChangeModel("")
                //                            tableview.checkAttackIconRow = -1
                //                            tableview.checkAttackIconColumn = -1
                //                            //tableview.selectedRow = model.row
                //                            //tableModel.selectionRow(tableview.selectedRow, 18)
                //                        } else if (decorate === "qrc:/Resources/battle-icon.jpg") {
                //                            //console.log(tableview.itemAtIndex(0,2))
                //                            tableModel.attacker(tableModel.data(tableview.index(
                //                                                                    model.row,
                //                                                                    2)))
                //                            tableModel.setChangeModel("attackerModel")
                //                            //tableview.model = tableModel ? tableModel : undefined
                //                            //change color attackIcon
                //                            tableview.checkAttackIconRow = 0
                //                            tableview.checkAttackIconColumn = column
                //                            selectionID.select(
                //                                        tableview.selectionModel.model.index(
                //                                            0, 0),
                //                                        ItemSelectionModel.ClearAndSelect | ItemSelectionModel.Rows)

                //                            //tableModel.selectionRow(row, 0)
                //                        }

                //                        if (decorate === "qrc:/Resources/target-icon.jpg") {
                //                            console.log(tableModel.data(tableview.index(
                //                                                            model.row, 2)))
                //                            tableview.index(0, 11)

                //                            //console.log(move)
                //                            //tableModel.moveAttackerToFirst(model.row)
                //                        }*/
                //                    }
                //                }
                //            }
                columnWidthProvider: function (column) {
                    //                    return 150
                    //                    var fixedLengs = tableview.columnZero + tableview.columnIcons * 6
                    //                    var columnProviderSize = (tableview.width - fixedLengs)
                    //                            / (tableModel.columnCount() - 6)
                    var columnProviderSize = 100
                    //console.log(rootItem.width / tableModel.columnCount())
                    if (column === 0) {
                        return tableview.columnZero
                    } else if (column === 1) {
                        return tableview.columnIcons
                    } else if (column === 2) {
                        return tableview.columnIcons * 2
                    } else {
                        return columnProviderSize
                    }
                }
                Menu {
                    id: menuTable
                    padding: 5
                    Repeater {
                        model: ["Go To", "Track"]

                        MenuItem {
                            id: menuItem
                            //implicitWidth: 89
                            //implicitHeight: 24
                            text: modelData
                            contentItem: RowLayout {
                                anchors.fill: parent
                                spacing: 2
                                IconImage {
                                    Layout.preferredHeight: 20
                                    Layout.preferredWidth: 20
                                    //color: "yellow"
                                    //Layout.leftMargin: 15 / rootItem.monitorRatio
                                    //Layout.rightMargin: 15 / rootItem.monitorRatio
                                    source: model.index === 0 ? "qrc:/Resources/goto-icon.jpg" : "qrc:/Resources/track-icon.jpg"
                                }
                                Text {
                                    //padding: 5
                                    //leftPadding: menuItem.indicator.width
                                    //rightPadding: menuItem.arrow.width
                                    text: modelData //menuItem.text
                                    //font: menuItem.font
                                    //opacity: enabled ? 1.0 : 0.3
                                    color: Style.foregroundColor //menuItem.highlighted ? "#ffffff" : "#21be2b"
                                    //horizontalAlignment: Text.AlignLeft
                                    font.family: Style.fontFamily
                                    font.pixelSize: Style.regularFontSize
                                    verticalAlignment: Text.AlignVCenter
                                    //elide: Text.ElideRight
                                }
                            }

                            background: Rectangle {
                                implicitWidth: 89
                                implicitHeight: 24
                                color: menuItem.highlighted ? rootItem.fg20 : "transparent"
                                radius: 10
                            }
                        }
                    }

                    background: Rectangle {
                        implicitWidth: 92
                        implicitHeight: 24
                        color: Style.backgroundColor
                        border.color: rootItem.fg20
                        radius: 10
                    }
                }
            }
        }

        Rectangle {
            id: sideFixedIconsContainer

            color: 'transparent'
            clip: true

            Layout.minimumWidth: 130
            Layout.fillHeight: true

            ListView {
                id: sideFixedIconsListView
                y: -tableview.contentY
                width: parent.width
                height: tableview.childrenRect.height
                model: tableview.childrenRect.height / tableview.tableCellHeight
                spacing: 1
                interactive: false

                delegate: Rectangle {
                    id: lvDelegate
                    property int lvIndex: model.index
                    width: sideFixedIconsListView.width
                    height: tableview.tableCellHeight
                    color: 'transparent'

                    Rectangle {
                        width: parent.width
                        height: 1
                        color: Style.foregroundColor
                        opacity: 0.2
                        anchors.bottom: parent.bottom
                    }

                    Row {
                        anchors.fill: parent
                        spacing: 2
                        Repeater {
                            id: iconsRepeater

                            model: ["qrc:/Resources/battle-icon.jpg", "qrc:/Resources/target-icon.jpg", "qrc:/Resources/goto-icon.jpg", "qrc:/Resources/track-icon.jpg"]
                            IconButton {
                                anchors.verticalCenter: parent.verticalCenter
                                width: 30 / Style.monitorRatio
                                height: 30 / Style.monitorRatio
                                iconImageSource: modelData
                                backgroundColor: "transparent"
                                onClicked: {
                                    if (tableview.isAttackecd) {
                                        tableview.isAttackecd = false
                                    } else {
                                        tableview.isAttackecd = true
                                    }
                                    if (index === 2) {
                                        tableModel.goToPosition(
                                                    lvDelegate.lvIndex)
                                    } else if (index === 3) {
                                        tableModel.trackPosition(
                                                    lvDelegate.lvIndex)
                                    }
                                }
                            }

                            // Rectangle {
                            //     width: 30
                            //     height: 30
                            //     color: 'transparent'

                            //     IconImage {
                            //         source: modelData
                            //         anchors.fill: parent
                            //     }

                            //     MouseArea {
                            //         anchors.fill: parent

                            //     }
                            // }
                        }
                    }
                }
            }
        }
    }
}
