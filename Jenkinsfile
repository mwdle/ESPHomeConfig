@Library("JenkinsPipelines") _ // See https://github.com/mwdle/JenkinsPipelines -- see compose.yaml

// `useBitwardenDefault: true` means this pipeline will always pull in a .env file from Bitwarden secure note that has the same name as the repository.
// If this behavior is not desired, simply create a Jenkinsfile in the repository that is the same as this one, but with `useBitwardenDefault: false`.
// `disableTriggers` ensures that this repository only builds when triggered manually, and will not build automatically when webhooks fire, or scans find changes.
dockerComposePipeline(useBitwardenDefault: true, disableTriggers: true) // This specific pipeline is dependent on the JenkinsBitwardenUtils shared library (https://github.com/mwdle/JenkinsBitwardenUtils) -- see compose.yaml