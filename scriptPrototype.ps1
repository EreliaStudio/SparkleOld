param (
    [string]$p_Directory,
    [string]$p_NewWord
)

# Vérifie si le répertoire existe
if (-Not (Test-Path $p_Directory -PathType Container)) {
    Write-Host "The specified directory does not exist."
    exit
}

# Parcourir les fichiers .hpp dans le répertoire et les sous-répertoires
Get-ChildItem -Path $p_Directory -Recurse -Include *.hpp | ForEach-Object {
    # Renommer le fichier en remplaçant "prototype" par p_NewWord
    $p_NewName = $_.Name -replace 'prototype', $p_NewWord

    # Vérifier si le nom a changé
    if ($_.Name -ne $p_NewName) {
        Rename-Item -Path $_.FullName -NewName $p_NewName
    }
}

Write-Host "Prototype files in $p_Directory have been renamed to $p_NewWord."
