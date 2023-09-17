function List-FilesRecursively {
    param (
        [string]$p_directoryPath,
        [string]$p_rootPath,
        [System.Collections.ArrayList]$p_generalGroup,
        [System.Collections.ArrayList]$p_openGLGroup,
        [System.Collections.ArrayList]$p_vulkanGroup
    )

    $p_items = Get-ChildItem -Path $p_directoryPath

    foreach ($p_item in $p_items) {
        $p_relativePath = $p_item.FullName.Substring($p_rootPath.Length)
        $p_formattedPath = "`"`${CMAKE_SOURCE_DIR}/includes/$p_relativePath;includes/$p_relativePath`""

        if ($p_relativePath -imatch "OpenGL") {
            $p_openGLGroup.Add($p_formattedPath) | Out-Null
        }
        elseif ($p_relativePath -imatch "Vulkan") {
            $p_vulkanGroup.Add($p_formattedPath) | Out-Null
        }
        else {
            $p_generalGroup.Add($p_formattedPath) | Out-Null
        }

        if (Test-Path $p_item.FullName -PathType Container) {
            List-FilesRecursively -p_directoryPath $p_item.FullName -p_rootPath $p_rootPath -p_generalGroup $p_generalGroup -p_openGLGroup $p_openGLGroup -p_vulkanGroup $p_vulkanGroup
        }
    }
}

$p_generalGroup = New-Object 'System.Collections.ArrayList'
$p_openGLGroup = New-Object 'System.Collections.ArrayList'
$p_vulkanGroup = New-Object 'System.Collections.ArrayList'

$p_currentDirectory = $PSScriptRoot
if (-not $p_currentDirectory) {
    $p_currentDirectory = Get-Location
}
if (-not $p_currentDirectory.EndsWith("\")) {
    $p_currentDirectory += "\"
}

List-FilesRecursively -p_directoryPath $p_currentDirectory -p_rootPath $p_currentDirectory -p_generalGroup $p_generalGroup -p_openGLGroup $p_openGLGroup -p_vulkanGroup $p_vulkanGroup

$p_generalWindows = $p_generalGroup | Where-Object {$_ -imatch "Windows"}
$p_generalLinux = $p_generalGroup | Where-Object {$_ -imatch "Linux"}
$p_generalOther = $p_generalGroup | Where-Object { -not ($_ -imatch "Windows") -and -not ($_ -imatch "Linux")}

$p_openGLWindows = $p_openGLGroup | Where-Object {$_ -imatch "Windows"}
$p_openGLLinux = $p_openGLGroup | Where-Object {$_ -imatch "Linux"}
$p_openGLOther = $p_openGLGroup | Where-Object { -not ($_ -imatch "Windows") -and -not ($_ -imatch "Linux")}

$p_vulkanWindows = $p_vulkanGroup | Where-Object {$_ -imatch "Windows"}
$p_vulkanLinux = $p_vulkanGroup | Where-Object {$_ -imatch "Linux"}
$p_vulkanOther = $p_vulkanGroup | Where-Object { -not ($_ -imatch "Windows") -and -not ($_ -imatch "Linux")}


Write-Host "General - Windows"
$p_generalWindows.ForEach({Write-Host $_})
Write-Host "`nGeneral - Linux"
$p_generalLinux.ForEach({Write-Host $_})
Write-Host "`nGeneral - Other"
$p_generalOther.ForEach({Write-Host $_})

Write-Host "`nOpenGL - Windows"
$p_openGLWindows.ForEach({Write-Host $_})
Write-Host "`nOpenGL - Linux"
$p_openGLLinux.ForEach({Write-Host $_})
Write-Host "`nOpenGL - Other"
$p_openGLOther.ForEach({Write-Host $_})

Write-Host "`nVulkan - Windows"
$p_vulkanWindows.ForEach({Write-Host $_})
Write-Host "`nVulkan - Linux"
$p_vulkanLinux.ForEach({Write-Host $_})
Write-Host "`nVulkan - Other"
$p_vulkanOther.ForEach({Write-Host $_})
