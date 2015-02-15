
public class HelloWorld {

		public void print(){
			System.out.println("HALLO");
		}
	public static void main(String args[]){
		final int currentTime = System.currentTimeMillis();
		System.out.println("Hello World: "+Integer.toString(currentTime));
		/*Thread t1 = new Thread() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				int counter = 0;
				int curtTime = System.currentTimeMillis();
				while(counter < 10){
					counter++;
					System.out.println("t1:"+counter+" : timeSpend:"+(System.currentTimeMillis() - curtTime));
					curtTime = System.currentTimeMillis();
					try{
						sleep(2000);
					}catch(Exception ex){
						System.out.println("t1 sleep error!");
					}
				}
			}
		};
		Thread t2 = new Thread() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				int counter = 0;
				while(counter < 10){
					counter++;
					System.out.println("t2:"+counter);
					try{
						sleep(500);
					}catch(Exception ex){
						System.out.println("t2 sleep error!");
					}
				}
			}
		};
		t1.start();
		t2.start();*/
		/*try{
			int a[] = new int[]{0,1,1,1};
			a=null;
			a[4] = 0;
			/*HelloWorld h = null;// new HelloWorld();
			h.print();* /
			System.out.println("After");
		}catch(ArrayIndexOutOfBoundsException ex){
			System.out.println("Exception: "+ ex.toString());
		}*/
		//System.out.println("Done");
	}
}
