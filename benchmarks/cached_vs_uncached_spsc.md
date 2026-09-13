# cached vs uncached spsc
## tabled data (claude gen)
### uncached
 
| run | p50 (us) | p99 (us) | p99.9 (us) | max (us) |
|---|---|---|---|---|
| 1 | 0.125 | 4.292 | 29.625 | 887.167 |
| 2 | 0.500 | 3.209 | 28.750 | 1142.375 |
| 3 | 0.125 | 3.208 | 32.917 | 1148.375 |
| 4 | 0.125 | 3.750 | 33.750 | 1508.125 |
| 5 | 0.125 | 3.041 | 27.625 | 1160.042 |
 
### cached
 
| run | p50 (us) | p99 (us) | p99.9 (us) | max (us) |
|---|---|---|---|---|
| 1 | 0.125 | 4.709 | 71.166 | 1470.834 |
| 2 | 0.125 | 3.042 | 175.458 | 8025.625 |
| 3 | 0.125 | 3.000 | 26.125 | 1049.792 |
| 4 | 0.125 | 3.084 | 28.000 | 1565.625 |
| 5 | 0.125 | 3.084 | 30.167 | 1296.834 |
 
### summary (mean across 5 runs)
 
| scenario | metric | uncached | cached | delta |
|---|---|---|---|---|
| saturated | p50 | 3395.70 us | 4054.90 us | +19.4% |
| saturated | p99 | 5820.91 us | 9808.86 us | +68.5% |
| saturated | p99.9 | 5854.40 us | 10053.88 us | +71.7% |
| saturated | max | 5859.63 us | 10076.44 us | +71.9% |
| paced | p50 | 0.20 us | 0.125 us | ~flat (1 outlier in uncached) |
| paced | p99 | 3.50 us | 3.38 us | ~flat |
| paced | p99.9 | 30.53 us | 66.18 us | +116.8% (driven by 1 outlier: run 2 = 175.458 us) |
| paced | max | 1169.22 us | 2681.74 us | +129.4% (driven by 1 outlier: run 2 = 8025.625 us) |

## raw data
### uncached
-- spsc queue --
[saturated run] order valid?: yes
p50: 3508.334 us
p99: 6006.792 us
p99.9: 6076.709 us
max: 6092.209 us
[paced run] order valid?: yes
p50: 0.125 us
p99: 4.292 us
p99.9: 29.625 us
max: 887.167 us

-- spsc queue --
[saturated run] order valid?: yes
p50: 3396.875 us
p99: 7041.333 us
p99.9: 7102.208 us
max: 7109.084 us
[paced run] order valid?: yes
p50: 0.500 us
p99: 3.209 us
p99.9: 28.750 us
max: 1142.375 us

-- spsc queue --
[saturated run] order valid?: yes
p50: 3202.417 us
p99: 5312.375 us
p99.9: 5333.416 us
max: 5334.208 us
[paced run] order valid?: yes
p50: 0.125 us
p99: 3.208 us
p99.9: 32.917 us
max: 1148.375 us

-- spsc queue --
[saturated run] order valid?: yes
p50: 3491.166 us
p99: 5375.250 us
p99.9: 5383.208 us
max: 5384.708 us
[paced run] order valid?: yes
p50: 0.125 us
p99: 3.750 us
p99.9: 33.750 us
max: 1508.125 us

-- spsc queue --
[saturated run] order valid?: yes
p50: 3379.708 us
p99: 5368.792 us
p99.9: 5376.458 us
max: 5377.959 us
[paced run] order valid?: yes
p50: 0.125 us
p99: 3.041 us
p99.9: 27.625 us
max: 1160.042 us


### cached
-- spsc queue --
[saturated run] order valid?: yes
p50: 4552.167 us
p99: 9122.084 us
p99.9: 9238.792 us
max: 9251.916 us
[paced run] order valid?: yes
p50: 0.125 us
p99: 4.709 us
p99.9: 71.166 us
max: 1470.834 us

-- spsc queue --
[saturated run] order valid?: yes
p50: 3755.000 us
p99: 9863.750 us
p99.9: 10239.334 us
max: 10240.209 us
[paced run] order valid?: yes
p50: 0.125 us
p99: 3.042 us
p99.9: 175.458 us
max: 8025.625 us

-- spsc queue --
[saturated run] order valid?: yes
p50: 3835.791 us
p99: 10195.500 us
p99.9: 10414.875 us
max: 10467.792 us
[paced run] order valid?: yes
p50: 0.125 us
p99: 3.000 us
p99.9: 26.125 us
max: 1049.792 us

-- spsc queue --
[saturated run] order valid?: yes
p50: 4007.209 us
p99: 9591.791 us
p99.9: 9758.333 us
max: 9772.625 us
[paced run] order valid?: yes
p50: 0.125 us
p99: 3.084 us
p99.9: 28.000 us
max: 1565.625 us

-- spsc queue --
[saturated run] order valid?: yes
p50: 4124.334 us
p99: 10271.167 us
p99.9: 10618.083 us
max: 10649.667 us
[paced run] order valid?: yes
p50: 0.125 us
p99: 3.084 us
p99.9: 30.167 us
max: 1296.834 us
