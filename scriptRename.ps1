# Fonction pour renommer les fichiers
Function Rename-Files {
    Param (
        [string]$p_directoryPath
    )

    # Obtient tous les fichiers du dossier et de ses sous-dossiers
    $files = Get-ChildItem -Path $p_directoryPath -Recurse -File

    # Parcourt chaque fichier
    foreach ($file in $files) {
        # Obtient le nom complet du fichier
        $fullPath = $file.FullName

        # Obtient le nom du fichier sans le chemin
        $fileName = $file.Name

        # Vérifie si la chaîne "__" est présente dans le nom du fichier
        if ($fileName -match "__") {
            # Remplace "__" par une chaîne vide
            $newFileName = $fileName -replace "__", "_"

            # Définit le nouveau chemin complet du fichier
            $newFullPath = [System.IO.Path]::Combine($file.Directory.FullName, $newFileName)

            # Renomme le fichier
            Rename-Item -Path $fullPath -NewName $newFullPath
            Write-Host "Renamed $fullPath to $newFullPath"
        }
    }
}

# Appelle la fonction sur le dossier courant
Rename-Files -p_directoryPath $PWD.Path
