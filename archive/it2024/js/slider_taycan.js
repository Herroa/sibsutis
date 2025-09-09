const imageList = [
  '../res/img/taycan_1.jpg',
  '../res/img/taycan_2.jpg',
  '../res/img/taycan_3.jpg',
  '../res/img/taycan_4.jpg',
  '../res/img/taycan_5.jpg',
  '../res/img/taycan_6.jpg',
  '../res/img/taycan_7.jpg',
  '../res/img/taycan_8.jpg',
  '../res/img/taycan_9.jpg',
  '../res/img/taycan_10.jpg',
  '../res/img/taycan_11.jpg',
  '../res/img/taycan_12.jpg',
  '../res/img/taycan_13.jpg',
  '../res/img/taycan_14.jpg',

]
const img = document.querySelector('.img')
const next = document.getElementById('next')
const previous = document.getElementById('previous')
var numImage = 0
img.src = imageList[numImage]

next.addEventListener('click', () => {
  numImage++
  if (numImage >= imageList.length) {
    numImage = 0
  }
  console.log(numImage >= imageList.length)
  console.log(numImage)
  img.src = imageList[numImage]
})
previous.addEventListener('click', () => {
  numImage--
  if (numImage < 0) {
    numImage = imageList.length - 1
  }
  console.log(numImage >= imageList.length)
  console.log(numImage)
  img.src = imageList[numImage]
})