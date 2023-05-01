# Creating Views for the Device Template

A "view" can be created and added to any device template that's already been imported into an IoT Central application. One view that is particularly useful is the "Properties" view which displays all of a device's read-only and writable properties on the same page. Writable properties are easily updated in this view. There are also "General" views that can be created by default to quickly visualize the telemetry data being received by the IoT Central application. Any of the default views that are automatically generated can certainly be customized. Whenever a view has been added/edited with a device template, the final step is to publish the device template so that the new changes take effect within your IoT Central application.

1. Using the left-hand navigation pane in the IoT Central application, click on `Connect` > `Device templates` and then click on the name of whatever device template that shows up in the list (there should be only one)

    <img src=".//media/views_img_01.png">

2. Using the left-hand navigation pane of the device template, click on `Views`

    <img src=".//media/views_img_02.png">
        
3. Click on the tile named `Editing device and cloud data` 

    <img src=".//media/views_img_03.png">

4. Type in "Properties" for the **Form Name**. Expand the **Properties** list and check all of the properties shown. Click on the **Add section** button, then the **Save** icon, then the **Back** icon

    <img src=".//media/views_img_04.png" width=200>

5. Using the left-hand navigation pane in the device template, click on **Views**

    <img src=".//media/views_img_02.png">

6. Click on the tile named `Generate default views` 

    <img src=".//media/views_img_03.png">

7. Click the button titled `Generate default dashboard view(s)` 

    <img src=".//media/views_img_08.png">

8. Click on the `Publish` icon (towards the top of the page) and a pop-up dialog box should appear; click on the `Publish` button

    <img src=".//media/views_img_09.png">
    <img src=".//media/views_img_10.png">

9. Using the left-hand navigation pane, access your device by selecting `Connect` > `Devices`. The new views which were just added (About, Overview, Properties) should appear and be ready for use!

    <img src=".//media/views_img_11.png">
    <img src=".//media/views_img_12.png">