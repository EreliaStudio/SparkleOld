Get-ChildItem -Path . -File -Recurse | Where-Object { $_.Name -match '^spk_(.*)$' } | ForEach-Object {
    $newName = $_.Name -replace '^spk_', 'spk_network_'
    $newFullPath = Join-Path -Path $_.Directory.FullName -ChildPath $newName
    Rename-Item -Path $_.FullName -NewName $newFullPath
}
