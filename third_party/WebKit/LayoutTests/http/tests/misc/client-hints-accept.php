<?php
    header("ACCEPT-CH: DPR, Width, Viewport-Width, Device-Memory");
?>
<!DOCTYPE html>
<script src="../resources/testharness.js"></script>
<script src="../resources/testharnessreport.js"></script>
<body>
    <script>
        var t = async_test('Client-Hints sent when Accept-CH header is present');
        var unreached = function() {
            assert_unreached("Image should have loaded.");
        };

        var loadDeviceMemoryImage = function() {
            var img = new Image();
            img.src = 'resources/image-checks-for-device-memory.php';
            img.onload = t.step_func(function(){ t.done(); });
            img.onerror = t.step_func(unreached);
            document.body.appendChild(img);
        };
        var loadRWImage = function() {
            var img = new Image();
            img.src = 'resources/image-checks-for-width.php';
            img.sizes = '500';
            img.onload = t.step_func(loadDeviceMemoryImage);
            img.onerror = t.step_func(unreached);
            document.body.appendChild(img);
        };
        var loadViewportImage = function() {
            var img = new Image();
            img.src = 'resources/image-checks-for-viewport-width.php';
            img.onload = t.step_func(loadRWImage);
            img.onerror = t.step_func(unreached);
            document.body.appendChild(img);
        };
        t.step(function() {
            var img = new Image();
            img.src = 'resources/image-checks-for-dpr.php';
            img.onload = t.step_func(loadViewportImage);
            img.onerror = t.step_func(unreached);
            document.body.appendChild(img);
        });
    </script>
</body>
