# Increment-PatchVersion.ps1

# Copyright (c) Bitstro Systems. All rights reserved.
# Licensed under BSDL 1.0

$VersionFile = Join-Path $PSScriptRoot 'version'

# writes a log message with a timestamp to the output
function Log-Message {
    param(
        [string] $Message
    )

    Write-Host "$(Get-Date -Format 'dd-MM-yyyy HH:mm:ss') $Message"
}

# reads a version string from a file (expects single line 'x.y.z')
function Read-VersionFile {
    param(
        [Parameter(Mandatory = $True)]
        [string] $Path
    )

    if (-not (Test-Path $Path)) {
        throw "Version file '$Path' does not exist."
    }

    $content = Get-Content $Path -ErrorAction Stop
    if ($content.Count -gt 1) {
        throw "Version file '$Path' has invalid format."
    }

    return $content.Trim()
}

# writes a version string to an existing file, replacing its contents
function Write-VersionFile {
    param(
        [Parameter(Mandatory = $True)]
        [string] $Path,

        [Parameter(Mandatory = $True)]
        [string] $Version
    )

    if (-not (Test-Path $Path)) {
        throw "Version file '$Path' does not exist."
    }

    Set-Content -Path $Path -Value $Version -Force -NoNewLine
}

# increments the patch component of a version string 'x.y.z' and returns the new version
function Increment-PatchVersion {
    param(
        [Parameter(Mandatory = $True)]
        [ValidatePattern('^\d+\.\d+\.\d+$')]
        [string] $Version
    )

    $components = $Version.Split(".")
    if (-not [int]::TryParse($components[2], [ref] $null)) {
        throw "Patch component '$($components[2])' is not numeric."
    }

    $components[2] = ([int] $components[2] + 1).ToString()
    return $components -join "."
}

try {
    Log-Message -Message "Reading current version from '$VersionFile'..."
    $currentVersion = Read-VersionFile -Path $VersionFile
    Log-Message -Message "Current version is $currentVersion"

    Log-Message -Message "Incrementing patch component..."
    $newVersion = Increment-PatchVersion -Version $currentVersion
    Log-Message -Message "New version is $newVersion"

    Log-Message -Message "Writing updated version to '$VersionFile'..."
    Write-VersionFile -Path $VersionFile -Version $newVersion
    Log-Message -Message "Version successfully updated"
} catch {
    Log-Message -Message "Error incrementing the patch version ($_)"
    exit 1
}